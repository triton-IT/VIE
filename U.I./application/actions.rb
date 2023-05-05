# frozen_string_literal: true

def action_router(id_found)
  # this is the main action methods that can debug sanitize and route to the corresponding method
  send("action_#{id_found}")
  # we set the context
  grab(:context).data[:mode] = id_found
  # if debug mode then we log the above messages
  return unless grab(:debug).data[:debug]

  puts "mode activated: #{grab(:context).data}"
  puts "function call : action_#{id_found}"
end

# tools actions
def action_load
  # grab(:context).data[:mode]=:loading
  # we send the command get_projects to the server
  Atome.controller_sender({ action: :get_projects })
  a = grab(:inspector)
  i = 0
  while i < 16
    b = Atome.new(
      { shape: { renderers: [:browser], id: "the_shape#{i}", type: :shape, attach: [:inspector], attached: [],
                 width: 50, height: 50,
                 color: { renderers: [:browser], id: :c31, type: :color, attach: ["the_shape#{i}"], attached: [],
                          red: 1, green: 0.15, blue: 0.15, alpha: 0.6 } } }
    )
    b.text({ data: "item#{i + 1}", visual: { size: 12 } })
    i += 1
  end

  a.sort(true) do |el|
    puts "just sorted  : #{el}"
  end
end

def action_clear
  alert 'clear'
  grab(:selected).data.each do |selected_slot|
    grab(selected_slot).materials.each do |child_found|
      grab(child_found)&.delete(true)
    end
  end
end

def action_settings
  inspector = grab(:inspector)
  clear_zone(inspector)
  item_to_list = [
    { label: :module_1, id: :p1 },
    { label: :module_2, id: :p2 },
    { label: :module_3, id: :p3 },
    { label: :module_4, id: :p4 },
    { label: :module_5, id: :p5, code: :test }
  ]
  inspector.sort(true) do |el|
    puts "just sorted  : #{el}"
  end

  # code will send the id of each item to the load method
  grab(:inspector).list({ listing: item_to_list, code: :load_modules,
                          style: { colors: %i[gray white], width: :auto, left: 0, right: 0, height: 15, items: { color: :orange, size: 36, align: :left, margin: 3 } } })
end

def action_edition
  inspector = grab(:inspector)
  # grab(:context).data[:mode]=:edition

  clear_zone(inspector)
  svg_color = 'rgba(180, 120, 50, 1)'
  margin = vie_styles[:margin]
  support_style = vie_styles[:support_style]
  icon_spacing = vie_styles[:support_style][:height] + margin * 2
  index = 0
  tool_list.each_value do |data|
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
      puts '-------'
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
      puts 'up -------'
      if @item_moved

        support.drag({ remove: true })
        `
      let element_id=#{id_found}
    let elem = document.getElementById(element_id);
elem.style.transform = "none";
`
        alert "1 : #{@prev_pos_left}"

        support.drag(move: true) do |_e|
          @item_moved = true
        end
      else

        alert "moved  is #{@item_moved}"
        send(action_found, id_found)

      end
      support.attach(:inspector)
      support.left(@prev_pos_left)
      support.top(@prev_pos_top)
      @item_moved = false
      alert "2 : #{support.left}"
    end
  end
end

def action_select
  # grab(:context).data[:mode]=:selection
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_group
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_copy
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_paste
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_undo
  inspector = grab(:inspector)
  clear_zone(inspector)
end

# sub actions
def action_get_modules
  Atome.controller_sender({ action: :get_modules })
end
