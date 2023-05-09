# frozen_string_literal: true

def cells_long_touch(module_slot)
  module_slot.touch(:long) do
    # if grab(:selected).data.include?(self.id)
    #   grab(:selected).data.each do |cell_id|
    #
    #     selected_cell = grab(cell_id)
    #     selected_cell.attached([:cell_color])
    #     selected_cell.detached(:active_color)
    #   end
    #   grab(:selected).data([])
    # else
    #   grab(:selected).data << self.id
    #   attached([:active_color])
    #   detached(:cell_color)
    # end
  end
end

def cell_touch_down(module_slot, index)
  module_slot.touch(:down) do
    log("down context is: #{grab(:context).data}")
    grab(:connection).data[0] = index
  end
end

def cells_over(module_slot)
  module_slot.over(:enter) do
    module_slot.detached(:cell_color)
    module_slot.attached(:cell_over)
  end

  module_slot.over(:leave) do
    module_slot.detached(:cell_over)
    module_slot.attached(:cell_color)
  end
end

def attach_cell_color(current_matrix, source_cell, target_cell)
  wait 0.6 do
    current_matrix.cell(source_cell).attached(:cell_color)
    current_matrix.cell(target_cell).attached(:cell_color)
    current_matrix.cell(source_cell).detached(:cell_connected)
    current_matrix.cell(target_cell).detached(:cell_connected)
  end
end

def detach_cell_color(current_matrix, source_cell, target_cell)
  current_matrix.cell(source_cell).detached(:cell_color)
  current_matrix.cell(target_cell).detached(:cell_color)
  current_matrix.cell(source_cell).attached(:cell_connected)
  current_matrix.cell(target_cell).attached(:cell_connected)
  attach_cell_color(current_matrix, source_cell, target_cell)
end

def cells_up_touch(module_slot, current_matrix, inspector, index)
  module_slot.touch(:up) do
    case context[:mode]

    when :default
      test_link_cell(current_matrix, inspector, index)
    when :edit
      # fill later
    when :select
      # fill later
    else
      log 'else for mouse up '
    end
  end
end

def cells_dropped_item(module_slot)
  module_slot.drop(true) do |event_content|
    log "received : #{event_content}"
  end
end

def matrix_events(current_matrix)
  inspector = grab(:inspector)
  current_matrix.cells.each_with_index do |module_slot_id, index|
    module_slot = grab(module_slot_id)
    cells_over(module_slot)
    cell_touch_down(module_slot, index)
    cells_long_touch(module_slot)
    cells_up_touch(module_slot, current_matrix, inspector, index)
    cells_dropped_item(module_slot)
  end
end

def tool_events(support, tool_name, options)
  support.touch(true) do
    action_router(tool_name, options)
  end
end
