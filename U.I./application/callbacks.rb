# frozen_string_literal: true

def get_projects_callback(msg_body)
  msg_body.each do |project|
    content = "project is : #{project.JS[:name]}\nid:  #{project.JS[:id]}\ndescription:  #{project.JS[:description]}"
    grab(:inspector).text({ data: content, top: 60, visual: { size: 12 } }) ##
  end
  return unless body.empty?

  grab(:inspector).text({ data: 'no project', top: 60, visual: { size: 12 } })
end

def get_project_callback(data)
  # project_id = data[:id]
  new_project_name = data[:name]
  grab(:current_project).data(new_project_name) # we update the project current project
  set_title
end

def get_modules_callback(body)
  log 'getting_display_svg'
  body.each do |module_found|
    content = "module is : #{module_found.JS[:name]}\nid:  #{module_found.JS[:id]}}"
    display_svg(module_found.JS[:icon], :lightgray, :get_modules_box)
    grab(:inspector).text({ data: content, top: 60, visual: { size: 12 } }) ##
  end
  return unless body.empty?

  grab(:inspector).text({ data: 'no project', top: 60, visual: { size: 12 }, width: 333 })
end

def place_module_on_slot(module_found)
  module_found.box({ id: "#{module_found.id}_svg_support", width: module_found.width / 2,
                     height: module_found.height / 2, center: true, attached: :invisible_color })
end

def insert_module_callback(module_id)
  grab(:selected).data.each do |module_id_found|
    module_found = grab(module_id_found)
    module_found.materials.each do |child_found|
      grab(child_found)&.delete(true)
    end
    tool_found = tool_list[module_id][:icon]
    tool_color = :orange
    place_module_on_slot(module_found)
    svg_fetch(tool_found, tool_color, "#{module_found.id}_svg_support")
  end
end
