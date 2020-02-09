#include "Keyboard.h"

Keyboard::Keyboard()
{

}

void Keyboard::OnKeyUp  (const std::function<void(const uint8_t)>& functor) { this->m_onKeyUpFunctors.push_back(functor);   }
void Keyboard::OnKeyDown(const std::function<void(const uint8_t)>& functor) { this->m_onKeyDownFunctors.push_back(functor); }

bool Keyboard::IsKeyDown(const uint8_t key)
{
	return this->m_downKeys.end() != std::find(this->m_downKeys.begin(), this->m_downKeys.end(), key);
}

void Keyboard::__OnWindowUpdateBegin()
{

}

bool Keyboard::__HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		const uint8_t keyCode = static_cast<uint8_t>(wParam);

		// Verify that the key was not down before the message was sent
		if (!CHECK_BIT(lParam, 30))
		{
			this->m_downKeys.push_back(keyCode);

			for (auto& functor : this->m_onKeyDownFunctors)
				functor(keyCode);
		}

		return true;
	}
	case WM_KEYUP:
	{
		const uint8_t keyCode = static_cast<uint8_t>(wParam);

		this->m_downKeys.erase(std::remove(this->m_downKeys.begin(), this->m_downKeys.end(), keyCode), this->m_downKeys.end());

		for (auto& functor : this->m_onKeyUpFunctors)
			functor(keyCode);

		return true;
	}
	}

	return false;
}

void Keyboard::__OnWindowUpdateEnd()
{

}