# frozen_string_literal: true

# additional atome's methods
class Atome
  def self.get_modules
    # log 'create the get_modules callback here'
    margin = vie_styles[:margin]
    module_style = vie_styles[:module_style]
    icon_spacing = vie_styles[:support_style][:height] + margin * 2

    index = 0
    fake_modules_list.each_value do |data|
      module_name=data[:icon]
      icon_found = get_icon(module_name)
      # action_found = data[:action]
      # id_found = data[:id]
      module_id = "module_#{module_name}"
      support = grab(:inspector).vector(module_style.merge({ definition: icon_found, top: (icon_spacing * index) + margin,
                                                             id: module_id }))
      label_found=module_id.sub('module_','').gsub('_',' ')
      label=grab(:inspector).text(module_style.merge({ top: (icon_spacing * index) + margin+12,left: 39,
                                                       width: :auto, data: label_found, visual: {size: 12},
                                                       id: "#{module_id}_label" }))
      index += 1

      support.depth(5)
      @item_moved = false
      support.drag(move: true) do |_e|
        @item_moved = true
      end

      support.touch(:down) do
        # id_found = support.id
        @prev_pos_left = support.left
        @prev_pos_top = support.top

      end

      support.touch(:up) do
        unless @item_moved
          alert :populate_selection
        end


      end

      support.drag(:start) do
        id_found = support.id
        left_found = 0
        top_found = 0
        `
let element_id=#{id_found}
let elem = document.getElementById(element_id);
let rect = elem.getBoundingClientRect();
#{left_found}=rect.x
#{top_found}=rect.y
`
        support.attach(:view)
        support.left(left_found)
        support.top(top_found)

      end
      support.drag(:end) do
        support.attach(:inspector)
        support.left(@prev_pos_left)
        support.top(@prev_pos_top)
        @item_moved = false
      end
    end
  end

  def self.controller_sender(message)
    action_found = message[:action]
    log "controller_sender, received: #{message}"
    case action_found

    when :new_project
      new_project_callback(fake_new_project_data)
    when :get_projects
      get_projects_callback(fake_project_list_data)
    when :load_project
      load_project_callback(fake_project_data)
    when :get_settings
      get_settings_callback
    when :get_modules
      Atome.get_modules
    else
      log 'controller_sender say : nothing to get'
    end
  end
end
