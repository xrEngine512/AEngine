#pragma once

#define RUS 0x1000
#define ENG 0x1100


#ifndef LOCALIZATION

#define LOCALIZATION RUS //by default

#endif

#if LOCALIZATION == RUS

#define OPEN_FILE_FAIL				L"�� ������� ������� ����"
#define FILE_ALREADY_OPENED			L"���� ��� ������"
#define COMPILATION_SUCCESS_QT		QStringLiteral("�������������� �������")
#define COMPILATION_FAIL_QT			QStringLiteral("������ �� ����� ����������")
#define SPECIFY_WORK_DIR_QT			QStringLiteral("����������, ������� ������� ����� �������. (���������->������� �����)")

//�������� ����������
#define CTRL_BTN_COMPILE_QT			QStringLiteral("�������������")
#define CTRL_BTN_SETTINGS_QT		QStringLiteral("���������")
#define CTRL_BTN_SAVE_QT			QStringLiteral("���������")
#define CTRL_BTN_LOAD_QT			QStringLiteral("���������")
#define SUB_CTRL_SAVE_SHADER_QT		QStringLiteral("���������������� ������")
#define SUB_CTRL_SAVE_PROJECT_QT	QStringLiteral("������ �������")

//�������
#define LBL_SETTINGS_QT				QStringLiteral("���������")
#define LBL_ENTRY_POINT_QT			QStringLiteral("����� �����")

//���������
#define TIP_HOLD_SIZE_QT			QStringLiteral("��������� ������ ���� (Scroll Lock)")
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

//ToolTips
#define TIP_HOLD_SIZE_QT			"Hold window size (Scroll Lock)"
#endif