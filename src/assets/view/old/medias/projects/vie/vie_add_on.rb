# atome add on

def cloner(atome_to_clone, attach_to_parent = nil)
  atome_properties = atome_to_clone.inspect
  atome_properties.delete(:atome_id)
  children = atome_properties.delete(:child)
  visual_found = atome_properties.delete(:visual)
  if visual_found
    size_found = visual_found[:size]
    atome_properties[:visual] = size_found.to_i
  end
  if attach_to_parent
    atome_properties.delete(:parent)
    atome_properties[:parent] = attach_to_parent
  end
  filtered_properties = {}
  atome_properties.each do |prop, value|
    filtered_properties[prop] = value if value
  end

  new_clone = Atome.new(filtered_properties)
  if children
    children.each do |child_found|
      cloner(grab(child_found), new_clone.atome_id)
    end
  end
  new_clone.state(false)
end

def list(items, style = {})
  style = { atome_id: :my_list, background: :transparent, line_background: :white, parent: :view, line_height: 33,
            line_spacing: 9, margin: 3, width: 99 }.merge(style)
  list = box({ color: style[:background], parent: style[:parent], atome_id: style[:atome_id], width: style[:width] })

  if style[:title]
    alert "style is #{style[:title]}"
  end

  if style[:line_shadow] && style[:line_shadow] == true
    style[:line_shadow] = { bounding: true }
  end

  items.each_with_index do |(data_id, content), index|
    item = list.box(true)
    item.data(data_id)
    item.shadow(style[:line_shadow])
    item.height(style[:line_height])
    item.width(list.width - (style[:margin] * 2))
    item.center(:x)
    item.y((style[:line_height] + style[:line_spacing]) * index + style[:line_spacing])
    item.color(style[:line_background])
    item.text({ content: content[:label][@language], xx: 6, y: 6, visual: style[:line_height] / 3 })
    item.image({ content: content[:icon], size: style[:line_height] * 0.6, center: :y, x: 3, z: 6 })

    # event below
    item.touch({ option: :down }) do |ev|
      vie_style = grab(:vie_style)
      duration = vie_style[:animation_duration]
      flash_color = vie_style[:flash_color]
      current_color = vie_style[:cell][:color]

      animate({
                start: { color: flash_color },
                end: { color: current_color },
                duration: duration * 1000,
                loop: 0,
                curve: :easing,
                target: atome_id
              })
    end
    # on drag start we duplicate the item
    item.touch(option: :up) do
      vie_style = grab(:vie_style)
      # here we feed the cell
      grab(:vie_matrix).data.cells.each do |cell|
        if cell.selected && state != :dragged
          fill_cell(cell, data_id, vie_style)
        end
      end
      state(false)
    end

    vie_style = grab(:vie_style)
    current_color = vie_style[:cell][:color]
    item.drag do |ev|
      if ev.start
        state(:dragged)
        color(current_color)
        cloner(self)
      end

      if ev.end
        delete
        state(false)
      end
    end
  end
end