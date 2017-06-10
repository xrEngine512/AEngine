//
// Created by islam on 19.03.17.
//

#include "ShaderCodeProcessorEnumerator.h"

#if defined(__VULKAN__)
#include "SPIRVShaderCodeProcessor.h"
#endif

#ifdef __D3D11__
#include "HLSLCodeProcessor.h"
#endif


using namespace std;

namespace ASL {

    std::vector<IShaderCodeProcessorPtr> enumerate_available_processors() {
        vector<IShaderCodeProcessorPtr> processors;

#ifdef __VULKAN__
        processors.push_back(make_shared<SPIRVShaderCodeProcessor>());
#endif

#ifdef __D3D11__
        processors.push_back(make_shared<HLSLCodeProcessor>());
#endif

        return processors;
    }

}
