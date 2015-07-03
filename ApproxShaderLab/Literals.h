#pragma once

#define RUS 0x1000
#define ENG 0x1100


#ifndef LOCALIZATION

#define LOCALIZATION RUS //by default

#endif

#if LOCALIZATION == RUS

#define OPEN_FILE_FAIL				L"Не удалось открыть файл"
#define FILE_ALREADY_OPENED			L"Файл уже открыт"
#define COMPILATION_SUCCESS_QT		QStringLiteral("Скомпилировано успешно")
#define COMPILATION_FAIL_QT			QStringLiteral("Ошибка во время компиляции")
#define SPECIFY_WORK_DIR_QT			QStringLiteral("Пожалуйста, укажите рабочую папку проекта. (Настройки->Рабочая папка)")

//Элементы управления
#define CTRL_BTN_COMPILE_QT			QStringLiteral("Компилировать")
#define CTRL_BTN_SETTINGS_QT		QStringLiteral("Настройки")
#define CTRL_BTN_SAVE_QT			QStringLiteral("Сохранить")
#define CTRL_BTN_LOAD_QT			QStringLiteral("Загрузить")
#define SUB_CTRL_SAVE_SHADER_QT		QStringLiteral("Скомпилированный шейдер")
#define SUB_CTRL_SAVE_PROJECT_QT	QStringLiteral("Проект шейдера")

//Надписи
#define LBL_SETTINGS_QT				QStringLiteral("Настройки")
#define LBL_ENTRY_POINT_QT			QStringLiteral("Точка входа")

//Подсказки
#define TIP_HOLD_SIZE_QT			QStringLiteral("Закрепить размер окна (Scroll Lock)")
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