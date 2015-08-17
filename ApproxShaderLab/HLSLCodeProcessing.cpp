#include "HLSLCodeProcessing.h"
#include "Session.h"
#include "ShaderPart.h"
#include <d3dcompiler.h>
#include <d3d11.h>
#include <string>
#include "TextureInfo.h"
#include "ShaderParamInfo.h"

using namespace std;
namespace ASL
{
    inline const char* addSM(Shader_Model sm)
    {
        switch (sm)
        {
        case V4_0: return "4_0";
        case V4_1: return "4_1";
        case V5_0: return "5_0";
        default:   return "";
        }
    }

	inline ShaderPackElementID toID(Shader_Type type)
	{
		switch (type)
		{
		case VS: return COMPILED_VS;
		case PS: return COMPILED_PS;
		case GS: return COMPILED_GS;
		case CS: return COMPILED_CS;
		case DS: return COMPILED_DS;
		case HS: return COMPILED_HS;
		default: return ID_NONE;
		}
	}

	inline const char* ShowNextWord(const ShaderPart &part)    //NOT ThreadSafe
    {
		static string resStr;
		resStr.clear();
		register char cur_symbol = part.CurrentSymbol();
		int l_pos = part.pos;

		while (cur_symbol != ' ' || resStr.size() < 1)
		{
			cur_symbol = part.Str_code[l_pos];
			if (cur_symbol != ' '&&cur_symbol != ';'&&cur_symbol != '\t'&&cur_symbol != '\n'&&cur_symbol != '\r')
			{
				resStr.push_back(cur_symbol);
			}
			else
			{
				l_pos++;
				if (resStr.size() == 0 && part.PosInRange())
					continue;
				return const_cast<char*>(resStr.c_str());
			}
			l_pos++;
			if (!part.PosInRange())
				break;
		}
		return const_cast<char*>(resStr.c_str());
    }

    inline const char* GetNextWord(ShaderPart &part)    //NOT ThreadSafe
    {
        static string resStr;
        resStr.clear();
		register char cur_symbol = part.CurrentSymbol();
		while (cur_symbol != ' ' || resStr.size() < 1)
        {        
			if (cur_symbol != ' '&&cur_symbol != ';'&&cur_symbol != '\t'&&cur_symbol != '\n'&&cur_symbol != '\r')
            {          
               resStr.push_back(cur_symbol);                
            }
            else
            {
                part.PosInc();
				cur_symbol = part.CurrentSymbol();
                if (resStr.size() == 0 && part.PosInRange())
                    continue;
                return const_cast<char*>(resStr.c_str());
            }
            part.PosInc();
			if (!part.PosInRange())
				break;
			cur_symbol = part.CurrentSymbol();
        }
        return const_cast<char*>(resStr.c_str());
    }

    inline DXGI_FORMAT GetFormat(char* type)
    {
        if (strcmp(type, "float") == 0)
            return DXGI_FORMAT_R32_FLOAT;
        if (strcmp(type, "float2") == 0)
            return DXGI_FORMAT_R32G32_FLOAT;
        if (strcmp(type, "float3") == 0)
            return DXGI_FORMAT_R32G32B32_FLOAT;
        if (strcmp(type,"float4") == 0)
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
    }

    inline bool IsNumber(char chr)
    {
        return chr >= '0' && chr <= '9' ? true : false;
    }

    inline unsigned int GetSemanticIndex(char* semanticNameWithIndex)
    {
        int i(0);
        while (semanticNameWithIndex[i] != '\0')
        {
            if (IsNumber(semanticNameWithIndex[i]))
            {
                char index = semanticNameWithIndex[i] - '0';
                semanticNameWithIndex[i] = '\0';
                return index;
            }
            i++;
        }
        return 0;
    }

    inline int FindNext(ShaderPart& part, const char * const keyword)  //returns index for str_code AFTER keyword
    {
        register char curr_char;
        string resStr; 

		while (part.PosInRange())
        {
            while (part.CurrentSymbol() != ' ' || resStr.size() < 1)
            {
				curr_char = part.CurrentSymbol();
                if (curr_char != ' '&& curr_char != ';'&& curr_char != '\t'&& curr_char != '\n'&& curr_char != '\r'&& curr_char != '('&& curr_char != ')')
                {
                    resStr.push_back(curr_char);
                }
                else
                {
					part.PosInc();
                    if (resStr.size() == 0)
                        continue;
                    if (strcmp(resStr.c_str(), keyword) == 0)
                        return part.pos;
                    resStr.clear();
                    continue;
                }
				part.PosInc();
            }
            if (strcmp(resStr.c_str(), keyword) == 0)
                return part.pos;
            resStr.clear();
        }
		return part.pos;
    }

	Errors HLSLCodeProcessing::Compile(Session* session)
	{
		ID3DBlob* shader;
		HRESULT res;
		int partIndex = 0;
		ShaderPart* part = session->partByIndex(partIndex);

		session->clearCompiledElements();

		session->writeElement(ShaderElement::fromC_Str(SHADER_NAME, session->ShaderName()));

		for (auto texture : session->ShaderTextures())
		{
			session->writeElement(ShaderElement::fromSaveData(TEXTURE_DESC, texture));
		}

		for (auto param : session->ShaderParameters())
		{
			session->writeElement(ShaderElement::fromSaveData(PARAM_DESC, param));
		}

		while (part)
		{
			const char *target = getTarget(part->Shader_Type, session->ShaderModel());
			res = D3DCompile(part->Str_code.c_str(), part->Str_code.size(), NULL, NULL, NULL, part->EntryPoint.c_str(), target, D3DCOMPILE_ENABLE_STRICTNESS, NULL, &shader, NULL);

			if (res == S_OK)
			{
				auto elem = ShaderElement::fromMemory(toID(part->Shader_Type), shader->GetBufferSize(), shader->GetBufferPointer());
				elem->AllowDuplicatesFor(RUNTIME_BUFFER_INFO);
				for (auto buffer : part->BuffersInfo)
				{
					*elem << ShaderElement::fromSaveData(RUNTIME_BUFFER_INFO, buffer);
				}
				
				int size;
				auto ptr = AbstractSaveData::Serialize(size, part->ParamsIDs, part->TextureSlots);
				*elem << ShaderElement::fromMemory(SHADER_SETS, size, ptr);
				
				session->writeElement(elem);
				

				if (part->Shader_Type == Shader_Type::VS)
				{
					auto ret = GenerateInputLayout(session, *part);
					if(ret != ASL_NO_ERROR)
					{
						return ret;
					}
				}
			}
			else
			{
				return ASL_COMPILATION_FAILED;
			}
			part = session->partByIndex(++partIndex);
		}
		return ASL_NO_ERROR;
	}

	const char* HLSLCodeProcessing::getTarget(Shader_Type type, Shader_Model sm)
	{
		const char* str_type;
		const char* str_ver = addSM(sm);
		switch (type)
		{
		case VS:  str_type = "vs_"; break;
		case PS:  str_type = "ps_"; break;
		case GS:  str_type = "gs_"; break;
		case CS:  str_type = "cs_"; break;
		case DS:  str_type = "ds_"; break;
		case HS:  str_type = "hs_"; break;
		default: throw exception("Unknown part type (In ApproxShaderLab)");
		}
		char *res = new char[strlen(str_type) + strlen(str_ver) + 1];
		strcpy(res, str_type);
		strcat(res, str_ver);
		return res;
	}



	Errors HLSLCodeProcessing::GenerateInputLayout(Session* session, ShaderPart& part)
	{
		register const char* str_code = part.Str_code.c_str();
		char* VSIName = nullptr;
		const char * const entryPoint = part.EntryPoint.c_str();
		const int size = part.Str_code.size();

		{
			int old_pos = part.pos;
			FindNext(part, entryPoint);
			const char* tmp = GetNextWord(part);
			VSIName = new char[strlen(tmp) + 1];
			strcpy(VSIName, tmp);                                 //store VSI Name
			part.pos = old_pos;
		}

		const char* curWord;
		char *data_type;
		while (part.PosInRange())
		{
			curWord = GetNextWord(part);
			if (strcmp(curWord, "struct") == 0)
			{
				curWord = GetNextWord(part);
				if (strcmp(curWord, VSIName) == 0)
				{
					delete[] VSIName;
					VSIName = nullptr;

					bool t(false);

					GetNextWord(part);                     //ignoring '{'
					do
					{
						curWord = GetNextWord(part);
						data_type = new char[strlen(curWord) + 1];
						strcpy(data_type, curWord);                 //store data type  
						GetNextWord(part);                 //ignoring name of variable
						GetNextWord(part);                 //ignoring ':'
						curWord = GetNextWord(part);       //semantic name
						D3D11_INPUT_LAYOUT_ELEMENT element;

						char *tmp = new char[strlen(curWord) + 1];
						
						strcpy(tmp, curWord);						//store semantic name 

						element.SemanticIndex = GetSemanticIndex(tmp);
						if (strcmp(tmp, "POSITION") == 0)
							element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
						else
							element.Format = GetFormat(data_type);

						delete[] data_type;

						data_type = nullptr;

						if (!t)
						{
							element.AlignedByteOffset = 0;
							t = true;
						}
						else
							element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
						element.InputSlot = 0;
						element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
						element.InstanceDataStepRate = 0;

						auto ILE = ShaderElement::fromObj(INPUT_LAYOUT_ELEMENT, element);
						*ILE << ShaderElement::fromC_Str(IL_SEMANTIC_NAME, tmp);
						
						delete[] tmp;
						tmp = nullptr;

						session->writeElement(ILE);

						curWord = ShowNextWord(part);
					} while (*curWord != '}');
					break;
				}
			}
		}
		return ASL_NO_ERROR;
	}


}