def init_vie
  style = grab(:vie_style)
  # layout
  box({ atome_id: :matrix_shell }.merge(style[:matrix_shell]))
  box({ atome_id: :inspector }.merge(style[:inspector]))
  box({ atome_id: :navigator }.merge(style[:navigator]))
  box({ atome_id: :action }.merge(style[:action]))
  box({ atome_id: :details }.merge(style[:details]))
  box({ atome_id: :title }.merge(style[:title]))

end

def activate_cell
  style = grab(:vie_style)
  duration = style[:animation_duration]
  style = grab(:vie_style)
  vie_playground = grab(:vie_matrix).data
  vie_playground.cells.over(false)
  selected(true)
  animate({
            start: { color: style[:cell][:color] },
            end: { color: style[:cell_selected][:color] },
            duration: duration * 1000,
            loop: 0,
            curve: :easing,
            target: atome_id
          })

  wait duration do
    set(style[:cell_selected])
  end
end

def deactivate_cell
  style = grab(:vie_style)
  duration = style[:animation_duration]
  style = grab(:vie_style)
  vie_playground = grab(:vie_matrix).data
  vie_playground.cells.over(false)
  selected(false)
  animate({
            start: { color: style[:cell_selected][:color] },
            end: { color: style[:cell][:color] },
            duration: duration * 1000,
            loop: 0,
            curve: :easing,
            target: atome_id
          })
  wait duration do
    set(style[:cell])
  end

end

def fill_cell(current_cell, module_id, vie_style)
  if current_cell.data
    current_cell.clear
  end
  current_cell.data(module_id)
  cell_label = current_cell.text(module_id)
  style_to_apply = vie_style[:cell]
  label_style = vie_style[:label]
  cell_label.set(style_to_apply)
  cell_label.set(label_style)
end

def select_cells(vie_playground, array_of_object, color = nil)
  array_of_object.each do |nb|
    my_cell = vie_playground.get_cell(nb)
    my_cell.activate_cell
  end
end

def resize_action(prent_object, playground, matrix_holder, ref_size)
  parent_height = prent_object.convert(:width)
  parent_width = prent_object.convert(:height)
  ratio = parent_width / parent_height
  if ratio > 1
    view_format = :landscape
  else
    view_format = :portrait
  end
  if view_format == :landscape
    size_to_apply = parent_height - (ref_size * 2)
    matrix_holder.size = size_to_apply
    playground.resize_matrix(size_to_apply)
    playground.matrix.center(true)
    matrix_holder.center(true)
  else
    size_to_apply = parent_width - (ref_size * 2)
    matrix_holder.size = size_to_apply
    playground.resize_matrix(size_to_apply)
    playground.matrix.center(true)
    matrix_holder.center(true)
  end
end

def resize_playground(playground)
  ref_size = grab(:vie_style)[:ref_size]
  prent_object = grab(:view)
  matrix_holder = grab(:matrix_shell)
  ATOME.resize_html do
    resize_action(prent_object, playground, matrix_holder, ref_size)
  end
  resize_action(prent_object, playground, matrix_holder, ref_size)
end

def playground_alert(matrix, alert_color = :red)
  matrix.cells.color(alert_color)
end

def new_playground(playground_id, &proc)
  cell_back = grab(:color_theme)[:base_color]
  style = grab(:vie_style)
  ref_size = style[:ref_size]
  background_color = style[:matrix_background]
  matrix_parent = :matrix_shell
  # playground
  vie_playground = Matrix.new({ parent: matrix_parent, nb: 8, size: 666, background_color: background_color, background: nil, atome_id: playground_id,
                                name: playground_id, margin: 9,
                                style: { color: cell_back, smooth: 3,
                                         border: { color: :black, thickness: 1, pattern: :solid } } })
  Atome.new({ atome_id: :vie_matrix, data: vie_playground })
  vie_playground.matrix.center(true)
  # elements

  # events/actions
  style = grab(:vie_style)
  duration = style[:animation_duration]
  flash_color = style[:flash_color]
  # TODO : on drop we should prevent to change the content of all activated cells
  vie_playground.cells.drop(true) do |_evt, ui, current_cell|
    dropped_element = `$(#{ui}.draggable).attr('id')`
    module_id = grab(dropped_element).data
    fill_cell(current_cell, module_id, style)
    animate({
              start: { color: current_cell.color },
              end: { color: flash_color },
              duration: duration * 1000,
              loop: 0,
              curve: :easing,
              target: current_cell.atome_id
            })
    wait duration * 1.3 do
      animate({
                start: { color: flash_color },
                end: { color: current_cell.color },
                duration: duration * 1000,
                loop: 0,
                curve: :easing,
                target: current_cell.atome_id
              })
    end

  end

  # events  for cells
  vie_playground.cells.touch({ option: :down }) do

    if selected
      @previous_state = :selected
    else
      @previous_state = :unselected

      if grab(:module_list).nil?
        proc.call
      end
      activate_cell
    end

    # lets inspect the selected cells and feed the details bar for natch treatment
    index = 0
    details_bar = grab(:details)
    if details_bar.child
      details_bar.child.delete
    end
    vie_playground.cells.each do |found_cell|
      if found_cell.selected && found_cell.data
        #TODO : add the style
        # now feed the details bar
        new_detail = details_bar.text(found_cell.data)
        new_detail.x = 99 * index + 99
        index = index + 1
      end

    end

  end
  vie_playground.cells.touch({ option: :up }) do |ev|
    if @previous_state == :selected
      deactivate_cell
    end
  end

  vie_playground.cells.touch({ option: :long }) do |ev|
    # we remove the over
    vie_playground.cells.touch({ option: :up }) do
      vie_playground.cells.over(false)
    end
    color1 = :green
    color2 = grab(:color_theme)[:base_color]
    if child
      if drag
        drag({ option: :remove })
        vie_playground.cells.over(false)
      else
        self.color(:black)
        virtual_event({ event: ev.to_n })
        vie_playground.cells.over(:enter) do
          self.color(color1)
        end
        vie_playground.cells.over(:exit) do
          self.color(color2)
        end
      end

    end

  end

  resize_playground(vie_playground)

  # logo
  vie_logo = image({ content: :vie_logo, xx: 9, y: 9, shadow: true, z: 6 })
  vie_logo.size(ref_size / 1.2)
  vie_playground

end

def new_project(data = {})
  style = grab(:vie_style)
  @language = data[:language]
  # first cleanup if theres a previous version
  grab(:vie_matrix).data.delete if grab(:vie_matrix)
  grab(:title).clear if grab(:title)
  matrix_name = data[:matrix]
  #lets create the title
  grab(:title).text({ content: data[:title] }.merge(style[:title_label]))
  vie_playground = new_playground(matrix_name) do
    list(data[:modules], data[:modules_style])
  end

  # lets create the action menu
  data[:actions].each_with_index do |(action_id, action_label), index|
    new_action = text({ id: action_id, content: action_label[@language] })
    new_action.set(data[:actions_style])
    new_action.x = 99 * index + 99
  end

  # let's create the atome that contain all the needed datas for the module
  data[:modules].each do |module_id, module_data|
    Atome.new({ atome_id: module_id, data: module_data })
  end

  # now lets create the matrix
  Atome.new({ atome_id: :vie_matrix, data: vie_playground })

  vie_playground
end
