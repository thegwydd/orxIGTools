#ifndef __ORX_IGTOOLS_PROPERTY_SLIDER_H__
#define __ORX_IGTOOLS_PROPERTY_SLIDER_H__

#include <Gwen/Controls/Properties.h>
#include <Gwen/Controls/PropertyTree.h>
#include <Gwen/Controls/Slider.h>

using namespace Gwen;
using namespace Gwen::Controls;
using namespace Gwen::Skin;


class PropertSlider : public Gwen::Controls::Property::Base
	{
	public:

		GWEN_CONTROL_INLINE(PropertSlider, Gwen::Controls::Property::Base)
			{
			m_Slider = new Gwen::Controls::HorizontalSlider(this);
			m_Slider->Dock(Gwen::Pos::Fill);
			m_Slider->onValueChanged.Add(this, &BaseClass::OnPropertyValueChanged);
			m_Slider->SetTabable(true);
			m_Slider->SetKeyboardInputEnabled(true);
			m_Slider->SetShouldDrawBackground(false);
			SetHeight(18);
			}

		virtual TextObject GetPropertyValue()
			{
			return m_Slider->GetValue();
			}

		virtual void SetPropertyValue(const TextObject & v, bool bFireChangeEvents)
			{
			m_Slider->SetValue(v.Get());
			}

		virtual bool IsEditing()
			{
			return m_Slider->HasFocus();
			}

		virtual bool IsHovered()
			{
			return BaseClass::IsHovered() || m_Slider->IsHovered();
			}

		Gwen::Controls::HorizontalSlider * GetSlider()
			{
			return m_Slider;
			}

		void SetRange(float min_val, float max_val) { m_Slider->SetRange(min_val, max_val); };
		void SetFloatValue(float val) { m_Slider->SetFloatValue(val); };

	protected:
		Gwen::Controls::HorizontalSlider * m_Slider;
	};



#endif //__ORX_IGTOOLS_PROPERTY_SLIDER_H__