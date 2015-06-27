#pragma once

#define RUS 0x1000
#define ENG 0x1100


#ifndef LOCALIZATION

#define LOCALIZATION RUS //by default

#endif

#if LOCALIZATION == RUS

#define OPEN_FILE_FAIL				L"�� ������� ������� ����"
#define FILE_ALREADY_OPENED			L"���� ��� ������"
#define COMPILATION_SUCCESS_QT		"�������������� �������"
#define COMPILATION_FAIL_QT			"������ �� ����� ����������"
#define SPECIFY_WORK_DIR_QT			"����������, ������� ������� ����� �������. (���������->������� �����)"

//�������� ����������
#define CTRL_BTN_COMPILE_QT			"�������������"
#define CTRL_BTN_SETTINGS_QT		"���������"
#define CTRL_BTN_SAVE_QT			"���������"
#define CTRL_BTN_LOAD_QT			"���������"
#define SUB_CTRL_SAVE_SHADER_QT		"���������������� ������"
#define SUB_CTRL_SAVE_PROJECT_QT	"������ �������"

//�������
#define LBL_SETTINGS_QT				"���������"			
#define LBL_ENTRY_POINT_QT			"����� �����"
#elif LOCALIZATION == ENG

#define OPEN_FILE_FAIL				L"Couldn't open file"
#define FILE_ALREADY_OPENED			L"File already opened"
#define COMPILATION_SUCCESS_QT		"Compilation Successful"
#define COMPILATION_FAIL_QT			"Compilation Failed"
#define SPECIFY_WORK_DIR_QT			"Please, specify project working directory. (Settings->Working directory)"

//Controls
#define CTRL_BTN_COMPILE_QT			"Compile"
#define CTRL_BTN_SETTINGS_QT		"Settings"
#define CTRL_BTN_SAVE_QT			"Save"
#define CTRL_BTN_LOAD_QT			"Load"
#define SUB_CTRL_SAVE_SHADER_QT		"Compiled shader"
#define SUB_CTRL_SAVE_PROJECT_QT	"Shader project"

//Labels
#define LBL_SETTINGS_QT				"Settings"
#define LBL_ENTRY_POINT_QT			"Entry point"	
#endif