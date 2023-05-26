# frozen_string_literal: true


def get_settings_callback
  log "viewing setings now"
end

def get_projects_callback(msg_body)
  # projects_list={}
  # msg_body.each do |project|
  #   projects_list[project[:id]]=project[:name]
  # end
  # # we fill user_projects atome with the projects_list
  # grab(:user_projects).data(projects_list)

  inspector = grab(:inspector)
   msg_body=[{ "name" => "new", "id" => "project_0", code: :new_project }].concat(msg_body)

  inspector.list({ listing: msg_body, code: :get_project, height: 23})
end


def new_project_callback(msg_body)
  new_project_name = msg_body[:name]
  grab(:current_project).data(new_project_name) # we update the project current project
  set_title
end

# def get_projects_callback(msg_body)
#   msg_body.each do |project|
#     content = "project is : #{project.JS[:name]}\nid:  #{project.JS[:id]}\ndescription:  #{project.JS[:description]}"
#     grab(:inspector).text({ data: content, top: 60, visual: { size: 12 } }) ##
#   end
#   return unless body.empty?
#
#   grab(:inspector).text({ data: 'no project', top: 60, visual: { size: 12 } })
# end

def load_project_callback(msg_body)
  modules_found= msg_body[:modules]
  links_found= msg_body[:links]
  clear_all_matrix
  modules_found.each do |module_data|
    add_module(module_data)
  end

  links_found.each do |link_data|
    add_link(link_data)
  end

end

def get_modules_callback(body)
  # log 'getting_display_svg'
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

# def insert_module_callback(module_id)
#   grab(:selected).data.each do |module_id_found|
#     module_found = grab(module_id_found)
#     module_found.materials.each do |child_found|
#       grab(child_found)&.delete(true)
#     end
#     tool_found = tool_list[module_id][:icon]
#     tool_color = :orange
#     place_module_on_slot(module_found)
#     svg_fetch(tool_found, tool_color, "#{module_found.id}_svg_support")
#   end
# end
