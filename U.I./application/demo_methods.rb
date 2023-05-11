# frozen_string_literal: true

# additional atome's methods
class Atome
  def self.get_modules
    log 'create the get_modules callback here'
    svg_color = 'rgba(180, 120, 50, 1)'
    margin = vie_styles[:margin]
    support_style = vie_styles[:support_style]
    icon_spacing = vie_styles[:support_style][:height] + margin * 2
    index = 0
    fake_modules_list.each_value do |data|
      icon_found = data[:icon]
      action_found = data[:action]
      id_found = data[:id]
      support = grab(:inspector).box(support_style.merge({ top: (icon_spacing * index) + margin,
                                                           id: "module_support_#{index}" }))
      svg_fetch(icon_found, svg_color, support.id)
      index += 1

      support.depth(5)
      @item_moved = false
      support.drag(move: true) do |_e|
        @item_moved = true
      end

      support.touch(:down) do
        id_found = support.id
        @prev_pos_left = support.left

        @prev_pos_top = support.top
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

      support.touch(:up) do
        if @item_moved

          support.drag({ remove: true })
          `
      let element_id=#{id_found}
    let elem = document.getElementById(element_id);
elem.style.transform = "none";
`
          log "1 : #{@prev_pos_left}"

          support.drag(move: true) do |_e|
            @item_moved = true
          end
        else

          log "moved  is #{@item_moved}"
          send(action_found, id_found)

        end
        support.attach(:inspector)
        support.left(@prev_pos_left)
        support.top(@prev_pos_top)
        @item_moved = false
        log "2 : #{support.left}"
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
