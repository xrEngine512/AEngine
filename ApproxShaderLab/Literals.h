#pragma once

#define RUS 0x1000
#define ENG 0x1100


#ifndef LOCALIZATION

#define LOCALIZATION RUS //by default

#endif

#if LOCALIZATION == RUS

#define OPEN_FILE_FAIL				L"Не удалось открыть файл"
#define FILE_ALREADY_OPENED			L"Файл уже открыт"
#define COMPILATION_SUCCESS_QT		"Скомпилировано успешно"
#define COMPILATION_FAIL_QT			"Ошибка во время компиляции"
#define SPECIFY_WORK_DIR_QT			"Пожалуйста, укажите рабочую папку проекта. (Настройки->Рабочая папка)"

//Элементы управления
#define CTRL_BTN_COMPILE_QT			"Компилировать"
#define CTRL_BTN_SETTINGS_QT		"Настройки"
#define CTRL_BTN_SAVE_QT			"Сохранить"
#define CTRL_BTN_LOAD_QT			"Загрузить"
#define SUB_CTRL_SAVE_SHADER_QT		"Скомпилированный шейдер"
#define SUB_CTRL_SAVE_PROJECT_QT	"Проект шейдера"

//Надписи
#define LBL_SETTINGS_QT				"Настройки"			
#define LBL_ENTRY_POINT_QT			"Точка входа"
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