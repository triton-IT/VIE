#include "processor_module.hpp"

#include "application.hpp"

using namespace nlohmann;

namespace live::tritone::vie
{
	processor_module::~processor_module()
	{		
	}
	
	void processor_module::initialize(nlohmann::json processor_definition) {
		for (auto& [index, parameter_definition] : processor_definition["parameters"].items()) {
			const unsigned long id = parameter_definition["id"];
			const std::string name = parameter_definition["name"];
			const std::string short_name = parameter_definition["short_name"];
			const std::string type = parameter_definition["type"];
			const std::string unit = parameter_definition["unit"];
			const float value = parameter_definition["value"];

			parameter parameter = application_.add_parameter(id, name, short_name, type, unit, value);

			set_parameter(parameter);
		}
	}

	uint_fast8_t processor_module::link(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id)
	{
		auto link = std::make_shared<module_link>();
		modules_links_[nb_modules_links_] = link;

		link->source_slot_id = source_slot_id;
		link->target_module = target_module;
		link->target_slot_id = target_slot_id;
		
		nb_modules_links_++;

		return nb_modules_links_;
	}

	uint_fast8_t processor_module::unlink(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id)
	{
		for (uint_fast8_t i = 0; i < nb_modules_links_ - 1; i++)
		{
			auto& link = modules_links_[i];
			if (link->source_slot_id == source_slot_id &&
				link->target_module == target_module &&
				link->target_slot_id == target_slot_id)
			{
				modules_links_[i] = modules_links_[nb_modules_links_];
				nb_modules_links_--;
				break;
			}
		}

		if (nb_modules_links_ > 0)
		{
			auto& link = modules_links_[nb_modules_links_ - 1];
			if (link->target_module == target_module)
			{
				nb_modules_links_--;
			}
		}

		return nb_modules_links_;
	}

	void processor_module::enable_link(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id)
	{
		for (uint_fast8_t i = 0; i < nb_modules_links_; i++)
		{
			auto& link = modules_links_[i];
			if (link->source_slot_id == source_slot_id &&
				link->target_module == target_module &&
				link->target_slot_id == target_slot_id)
			{
				link->enabled = true;
				break;
			}
		}
	}

	void processor_module::disable_link(uint_fast8_t source_slot_id, std::shared_ptr<processor_module> target_module, uint_fast8_t target_slot_id)
	{
		for (uint_fast8_t i = 0; i < nb_modules_links_; i++)
		{
			auto& link = modules_links_[i];
			if (link->source_slot_id == source_slot_id &&
				link->target_module == target_module &&
				link->target_slot_id == target_slot_id)
			{
				link->enabled = false;
				break;
			}
		}
	}

	void processor_module::unlink(std::shared_ptr<processor_module> target_module)
	{
		for (uint_fast8_t i = 0; i < nb_modules_links_ - 1; i++)
		{
			auto& link = modules_links_[i];
			if (link->target_module == target_module)
			{
				modules_links_[i] = modules_links_[nb_modules_links_];
				nb_modules_links_--;
			}
		}

		if (nb_modules_links_ > 0)
		{
			auto& link = modules_links_[nb_modules_links_ - 1];
			if (link->target_module == target_module)
			{
				nb_modules_links_--;
			}
		}
	}
	
	std::array<std::shared_ptr<module_link>, 32>& processor_module::get_modules_links(uint_fast8_t& nb_module_links)
	{
		nb_module_links = nb_modules_links_;
		return modules_links_;
	}
}