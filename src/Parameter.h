#pragma once

#include "IParameterListener.h"

namespace live::tritone::vie {
	class Parameter
	{
	public:
		Parameter(
			unsigned long id,
			wchar_t title[128],
			wchar_t shortTitle[128],
			wchar_t units[128],
			long stepCount,
			double defaultNormalizedValue,
			long unitId,
			long flags);

		Parameter(const Parameter& other);

		unsigned long getId();
		void getTitle(wchar_t out[128]);
		void getShortTitle(wchar_t out[128]);
		void getUnits(wchar_t out[128]);
		long getStepCount();
		double getDefaultNormalizedValue();
		long getUnitId();
		long getFlags();

		double getNormalizedValue();
		bool setNormalizedValue(double normalizedValue);

		virtual double toPlainValue(double valueNormalized);
		virtual double toNormalizedValue(double plainValue);

		virtual void toString(double normalizedValue, wchar_t string[128]);
		virtual bool fromString(const wchar_t* string, double& normalizedValue);

		virtual void setListener(IParameterListener* listener);

	private:
		unsigned long id_;
		wchar_t title_[128];
		wchar_t shortTitle_[128];
		wchar_t units_[128];
		long stepCount_;
		double defaultNormalizedValue_;
		long unitId_;
		long flags_;			//DAW dependent parameters

		double normalizedValue_;

		IParameterListener* listener_;
	};
}