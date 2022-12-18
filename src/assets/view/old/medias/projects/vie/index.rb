# # Vie UI v0.1
require './www/public/medias/projects/vie/vie_utils.rb'
require './www/public/medias/projects/vie/vie_add_on.rb'
require './www/public/medias/projects/vie/vie_style.rb'
# require './www/public/medias/projects/vie/slider.rb'

# data set

modules = { test: { data: :wave, icon: :wait, label: { english: :sin, french: :arr }, type: :unit,
                    value: 0.2, parameter: :frequency, ins: [], outs: [] },
            utils: { data: :filter, icon: :run, label: { english: :help, french: :aide }, type: :unit },
            hello: { data: :hello, icon: :action, label: { english: :action, french: :utils }, type: :container,
                     children: [:matrix_2, :matrix_3], parameters: { matrix_2: [:matrix_2_3] } } }

modules_style = { atome_id: :module_list, parent: :navigator, width: grab(:vie_style)[:ref_size],
                  background: :transparent, line_background: grab(:color_theme)[:base_color],
                  line_shadow: true }

#cells to be fill at startup

cells = { basis_1: {}, basis_6: {} }

actions = { deselect: { french: :deselect, english: :unselect },
            delete: { english: :clear, french: :suppr },
            link: { english: :link, french: :lier },
            copy: { english: :copy, french: :copier },
            paste: { english: :paste, french: :coller },
            group: { english: :group, french: :grouper },
            ungroup: { english: :ungroup, french: "dégrouper" },
            undo: { english: :undo, french: :annuler },
            redo: { english: :redo, french: :retablir },

}

actions_style = { atome_id: :action_list, parent: :action, color: grab(:color_theme)[:text_color], y: 33, visual: 15 }

# test projects
test_project = { title: "SampleMachine",
                 language: :english,
                 matrix: :basis,
                 matrices: [:basis, :matrix2],
                 cells: cells,
                 modules: modules,
                 modules_style: modules_style,
                 actions: actions,
                 actions_style: actions_style,
}

test_project2 = { title: "SampleModule",
                 language: :english,
                 matrix: :basis,
                 matrices: [:mymat1, :mymat2],
                 cells: cells,
                 modules: modules,
                 modules_style: modules_style,
                 actions: actions,
                 actions_style: actions_style,
}

# init now
init_vie

# create new project
new_project(test_project)
# wait 3 do
#   new_project(test_project2)
#   # alert :ok
# end

# DONE: create the json
# Done : create auto enlarge slider
# Done create a container for the to : allow the selection of whatever the matrix id and allow scroll overflow
# TODO : push to github
# Done: create shortcut

# json test

def send_to_engine(params)
  puts params
end

# get_projects , get all available of current user
projects = :get_projects
send_to_engine projects.to_json

# new_project
new = { new_project: :my_project }
send_to_engine new.to_json

# save
## deprecated ##

# save_as
save = { save_as: :my_new_project }
send_to_engine save.to_json

# load a project
load = { load: :my_project }
send_to_engine load.to_json

# get_modules list
modules = :get_modules
send_to_engine modules.to_json

# add_module : place module on the matrix
active_module = { sin: :my_matrix_3 }
send_to_engine active_module.to_json

# set_module_value
set_module_value = { my_matrix_3: 0.1 }
send_to_engine set_module_value.to_json

# link_modules
link_modules = { my_matrix_3: :frequency, my_matrix_6: :lfo }
send_to_engine link_modules.to_json

# set_module_name
set_module_name = "my module"
send_to_engine set_module_name.to_json

# undo
undo = :undo
send_to_engine undo.to_json

# redo
re_do = :redo
puts re_do.to_json

# current_matrix=grab(:matrix_shell)
grab(:view).key({ options: :down }) do |evt|
  if evt.ctrl_key
    evt.stop_propagation
    evt.stop_immediate_propagation
    evt.prevent_default
    case evt.key_code
    when 65
      # alert current_matrix.child[0]
      activated=nil
      current_matrix=grab(:vie_matrix).data
      current_matrix.cells.each do |cell|
        if cell.selected
          activated=true
        end
      end
      if activated
        current_matrix.cells.each do |cell|
          cell.deactivate_cell
        end
      else
        current_matrix.cells.each do |cell|
          cell.activate_cell
        end
      end
      else
      end
  end
end
