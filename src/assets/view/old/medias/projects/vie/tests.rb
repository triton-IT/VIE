require './www/public/medias/projects/vie/index.rb'


# tests
# my_cell = vie_playground.get_cell(9)
# my_cell.activate

# Select test
# wait 1 do
#   matrix= grab(:vie_matrix).data
#   select_cells(matrix, [1, 9, 12], { red: 0.6, green: 0, blue: 0, alpha: 1 })
# end

# # tag test
# vie_playground.get_cell(9).tag({ my_tag: :checked })
# vie_playground.get_cell(9).tag({ select: false })

# clean test
# wait 3 do
#   new_project(vie_playground, {})
# end
# vie_playground.cells.x(33)
# delete test
# wait 2 do
#   vie_playground.delete
# end

# # alert test
#
# wait 2 do
#   playground_alert(vie_playground, :orange)
# end
# #

# zoom test

# my_cell = vie_playground.get_cell(9)
# my_cell.width = my_cell.height = 120
#
# @matrix.width = matrix_size+@margin
# @matrix.height = matrix_size+@margin

# zoom test
# wait 1 do
#   vie_playground.zoom("item", "percent")
# end



################ new tests


# # # b=box
# # #
# # # b.touch({option: :up}) do
# # #   alert :ggo
# # # end
# # #
# # # alert b.touch[:proc]

# def cloning(obj)
#
# end
# b=box(atome_id: :the_box)
# b.drag(true) do |ev|
#   if ev.start
#     puts 'okok'
#   end
# end
# b.drag[:proc]=nil
# b.drag(true) do |ev|
#   if ev.start
#     puts 'poipo'
#   end
# end
# b.drag(true)
# b.touch ({ option: :down }) do
# puts :down1
# end
#
# b.touch ({ option: :down }) do
#   puts :down2
# end
#
# b.touch ({ option: :down }) do
#   puts :down3
# end
#
# b.touch ({ option: :up }) do
#   puts :up1
# end
#
# b.touch ({ option: :up }) do
#   puts :up2
# end
#
# b.touch ({ option: :up }) do
#   puts :up3
# end
# b.touch(:remove)
# b.drag(true)
# recup=b.drag
# grab(:the_box).drag(:disable)
# grab(:the_box).drag(:destroy)
# alert b.drag
# # grab(:the_box).drag(true)
#
# b.drag(recup)

# b.drag(true) do |ev|
#   if ev.start
#     puts 'poilu'
#   end
# end

# # wait 6 do
#   `console.clear()`
#   puts '---------'
#   puts b.drag
# # end
#
# #
# b.touch ({ option: :up }) do
#   puts :llllol
# end

