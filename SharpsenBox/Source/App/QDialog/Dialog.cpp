#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "IComponent.hpp"
#include "IGameManager.hpp"

namespace sb
{
	std::string Dialog::GetName()
	{
		return TYPENAME(Dialog);
	}

	void Dialog::Update() {}

	void Dialog::Init() {}

	Dialog::Dialog() {}

	Dialog::~Dialog() {}

	void Dialog::SetType(Type t)
	{
		_Type = t;
	}

	void Dialog::SetInfo(QString str)
	{
		_Info = str;
	}

	void Dialog::Show()
	{
		ShowDialog();
	}

	bool Dialog::GetValue()
	{
		return _Value;
	}

	void Dialog::SetCallback(const std::function<bool(bool)> &cb)
	{
		_Callback = cb;
	}

	int Dialog::GetDialogType()
	{
		return static_cast<int>(_Type);
	}

	QString Dialog::GetInfo()
	{
		return _Info;
	}

	Q_INVOKABLE void Dialog::Resolve(bool value)
	{
		_Value = value;
		bool dontHide = false;
		if (_Callback && _Type != Type::INFO)
			dontHide = _Callback(value);
		if (!dontHide)
		{
			ResetCallback();
			_Type = NONE;
		}
		ShowDialog();
	}

	void Dialog::ResetCallback()
	{
		_Callback = nullptr;
	}
}