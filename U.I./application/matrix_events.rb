# frozen_string_literal: true

def matrix_events(current_matrix)
  inspector = grab(:inspector)
  current_matrix.cells.each_with_index do |module_slot_id,index|
    module_slot = grab(module_slot_id)
    module_slot.over(:enter) do
      module_slot.detached(:cell_color)
      module_slot.attached(:cell_over)
    end

    module_slot.over(:leave) do
      module_slot.detached(:cell_over)
      module_slot.attached(:cell_color)
    end

    current_matrix.cell(index).touch(:down) do
      puts "down context is: #{grab(:context).data}"
      grab(:connection).data[0] = index
    end
    current_matrix.cell(index).touch(:long) do
      # puts "up context is: #{grab(:context).data}"
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
    current_matrix.cell(index).touch(:up) do
      alert "create a decison hub conditioned to the context "
      grab(:connection).data[1] = index
      source_cell = grab(:connection).data[0]
      target_cell = grab(:connection).data[1]
      unless source_cell == target_cell
        clear_zone(inspector)
        new_connection = grab(:connection).data
        inspector.text(data: "linking : #{new_connection[0]} and #{new_connection[1]}", visual: { size: 12 })
        current_matrix.cell(source_cell).detached(:cell_color)
        current_matrix.cell(target_cell).detached(:cell_color)
        current_matrix.cell(source_cell).attached(:cell_connected)
        current_matrix.cell(target_cell).attached(:cell_connected)
        wait 0.6 do
          current_matrix.cell(source_cell).attached(:cell_color)
          current_matrix.cell(target_cell).attached(:cell_color)
          current_matrix.cell(source_cell).detached(:cell_connected)
          current_matrix.cell(target_cell).detached(:cell_connected)
        end

      end
      alert grab(:connection).data
    end

    current_matrix.cell(index).drop(true) do |event_content|
      puts "reveived : #{event_content}"

    end

  end



end

