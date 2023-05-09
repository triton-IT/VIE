# frozen_string_literal: true

def context
  grab(:context).data
end

def debug
  grab(:debug).data[:debug]
end

def log(val)
  puts "=> #{val}" if debug
end

def link_cells(current_matrix, inspector, source_cell, target_cell)
  new_connection = grab(:connection).data
  inspector.text(data: "linking : #{new_connection[0]} and #{new_connection[1]}", visual: { size: 12 })
  detach_cell_color(current_matrix, source_cell, target_cell)
end

def test_link_cell(current_matrix, inspector, index)
  grab(:connection).data[1] = index
  source_cell = grab(:connection).data[0]
  target_cell = grab(:connection).data[1]
  return if source_cell == target_cell

  clear_zone(inspector)
  link_cells(current_matrix, inspector, source_cell, target_cell)
end
