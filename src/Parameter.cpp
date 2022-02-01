#include "Parameter.h"

#include <cwchar>

namespace live::tritone::vie {
	Parameter::Parameter(
		unsigned long id,
		wchar_t title[128],
		wchar_t shortTitle[128],
		wchar_t units[128],
		long stepCount,
		double defaultNormalizedValue,
		long unitId,
		long flags)
		: listener_(nullptr),
		id_(id),
		stepCount_(stepCount),
		defaultNormalizedValue_(defaultNormalizedValue),
		unitId_(unitId),
		flags_(flags),
		normalizedValue_(0)
	{
		wcscpy(title_, title);
		wcscpy(shortTitle_, shortTitle);
		wcscpy(units_, units);
	}

	Parameter::Parameter(const Parameter& other)
		: listener_(other.listener_),
		id_(other.id_),
		stepCount_(other.stepCount_),
		defaultNormalizedValue_(other.defaultNormalizedValue_),
		unitId_(other.unitId_),
		flags_(other.flags_),
		normalizedValue_(0)
	{
		wcscpy(title_, other.title_);
		wcscpy(shortTitle_, other.shortTitle_);
		wcscpy(units_, other.units_);
	}

	unsigned long Parameter::getId() {
		return id_;
	}

	void Parameter::getTitle(wchar_t out[128]) {
		wcscpy(out, title_);
	}

	void Parameter::getShortTitle(wchar_t out[128]) {
		wcscpy(out, shortTitle_);
	}

	void Parameter::getUnits(wchar_t out[128]) {
		wcscpy(out, units_);
	}

	long Parameter::getStepCount() {
		return stepCount_;
	}

	double Parameter::getDefaultNormalizedValue() {
		return defaultNormalizedValue_;
	}

	long Parameter::getUnitId() {
		return unitId_;
	}

	long Parameter::getFlags() {
		return flags_;
	}

	double Parameter::getNormalizedValue() {
		return normalizedValue_;
	}

	bool Parameter::setNormalizedValue(double normalizedValue) {
		bool modified = false;

		if (normalizedValue_ != normalizedValue)
		{
			normalizedValue_ = normalizedValue;

			if (listener_ != nullptr) {
				listener_->parameterValueChanged(id_, normalizedValue_);
			}

			modified = true;
		}

		return modified;
	}

	double Parameter::toPlainValue(double valueNormalized) {
		return valueNormalized;
	}

	double Parameter::toNormalizedValue(double plainValue) {
		return plainValue;
	}

	void Parameter::toString(double normalizedValue_, wchar_t string[128])
	{
		swprintf(string, L"%.4f", normalizedValue_);
	}

	bool Parameter::fromString(const wchar_t* string, double& normalizedValue)
	{
		int nbItems = swscanf(string, L"%.4f", &normalizedValue);
		return nbItems > 0;
	}

	void Parameter::setListener(IParameterListener* listener) {
		listener_ = listener;
	}
}
