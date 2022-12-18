def close_slider(back, max_size, start_value, smooth_in_px, axis, duration, property)
  if !state[:animating] && state[:slider] == :open
    cursor_back_id = "cursor_back_#{atome_id}"
    back.state[:animating] = true
    end_position = -max_size + start_value
    animate({
              start: { property => max_size, smooth: smooth_in_px, axis => end_position },
              end: { property => start_value, smooth: smooth_in_px, axis => 0 },
              duration: duration * 1000,
              loop: 0,
              curve: :easing,
              target: atome_id
            })
    animate({
              start: { opacity: 0 },
              end: { opacity: 1 },
              duration: duration * 1000,
              loop: 0,
              curve: :easing,
              target: "icon_#{atome_id}"
            })
    animate({
              start: { opacity: 1 },
              end: { opacity: 0 },
              duration: duration * 1000,
              loop: 0,
              curve: :easing,
              target: cursor_back_id
            })

  end
  wait duration do
    back.set(property => start_value)
    back.state[:slider] = :closed
    back.state[:animating] = false
  end
end

def open_slider(back, start_value, smooth_in_px, max_size, axis, duration, property, cursor_back_id)
  back.drag({ fixed: true }) do |evt|
    if evt.offset_y < -9 && !back.state[:animating] && back.state[:slider] == :closed
      back.state[:animating] = true
      end_position = -max_size + start_value
      animate({
                start: { property => start_value, smooth: smooth_in_px, axis => 0 },
                end: { property => max_size, smooth: smooth_in_px, axis => end_position },
                duration: duration * 1000,
                loop: 0,
                curve: :easing,
                target: atome_id
              })
      animate({
                start: { opacity: 1 },
                end: { opacity: 0 },
                duration: duration * 1000,
                loop: 0,
                curve: :easing,
                target: "icon_#{atome_id}"
              })
      animate({
                start: { opacity: 0 },
                end: { opacity: 1 },
                duration: duration * 1000,
                loop: 0,
                curve: :easing,
                target: cursor_back_id
              })
      wait duration do
        back.set(property => max_size)
        back.state[:animating] = false
        back.state[:slider] = :open
      end
    end

  end
end

def cursor(params, back, back_atome_id, cursor_size, cursor_position, max_size, start_value, smooth_in_px, axis, duration, property, cursor_back_id)
  value_atome = grab("value_#{back_atome_id}")
  cursor_back = back.box({ atome_id: "cursor_back_#{back_atome_id}",
                           height: :auto, y: params[:max] / 15, yy: params[:max] / 15,
                           width: params[:back][:size] / 3, center: :x, smooth: params[:max] / 100, shadow: { invert: true },
                           color: params[:cursor][:back_color], opacity: 0 })

  cursor = cursor_back.box({ shadow: { bounding: true }, smooth: cursor_size / 3,
                             height: cursor_size, width: cursor_size * 1.2,
                             color: params[:cursor][:color], atome_id: "cursor_#{back_atome_id}",
                             y: 0 })

  cursor.center(:x)
  cursor.yy(cursor_position)
  cursor_back = grab("cursor_back_#{back_atome_id}")
  cursor.drag({ lock: :x, containment: :true }) do |ev|

    current_value = (cursor.yy / (cursor_back.convert(:height) - cursor.height) * 100.to_i).ceil
    # we store the current value into the cursor
    back.data(current_value)
    value_atome.content(current_value)
    value_atome.center(x)
    value_atome.set(y: start_value / 12)
  end

end

def slider(params)
  property = ''
  start_value = ''
  axis = ''
  back_params = params[:back]
  duration = params[:duration]
  orientation = params[:orientation]
  back_atome_id = back_params[:atome_id]
  value_requested = params[:cursor][:value]
  unit = params[:cursor][:unit]
  end_range = params[:cursor][:range][1]
  max_size = params[:max]
  cursor_position = end_range * value_requested * params[:max] / (end_range)
  # let's create the background
  back = box(back_params.merge({z: 3}))
  back.data(value_requested)
  offset = (back.size[:value] * 30 / 100)
  back.over(:enter) do
    back.z(6)
  end
  back.over(:exit) do
    back.z(3)
  end
  back_size = back.size[:value]
  cursor_size = back_size * 30 / 100
  smooth_in_px = (back.smooth.sub('%', '').to_f / back.width) * 100
  cursor_back_id = "cursor_back_#{back_atome_id}"

  # let's create the icon
  icon = back.image({ content: params[:icon], atome_id: "icon_#{back_atome_id}" })
  icon.size = back.size[:value] - offset * 2
  icon.center(true)

  if orientation == :vertical
    property = :height
    start_value = back.height
    axis = :y
  elsif orientation == :horizontal
    property = :width
    start_value = back.width
  else
    # circular
  end

  # let's create the value
  back.text({ content: value_requested, atome_id: "value_#{back_atome_id}", y: back_size / 12 }.merge(params[:label]))

  # let's create the unit
  unit_atome = back.text(params[:label].merge({ content: unit, atome_id: "unit_#{back_atome_id}", yy: back_size / 60 }))
  wait 0.1 do
    unit_atome.yy(back_size / 12)
  end

  # events below
  # let's create the cursor

  back.state = { animating: false, slider: :closed }
  cursor(params, back, back_atome_id, cursor_size, cursor_position, max_size, start_value, smooth_in_px, axis, duration, property, cursor_back_id)

  # on touch
  back.touch({ option: :up }) do
    close_slider(back, max_size, start_value, smooth_in_px, axis, duration, property)
  end

  # on drag
  open_slider(back, start_value, smooth_in_px, max_size, axis, duration, property, cursor_back_id)
  back
end

theme_color = { red: 1, blue: 0, green: 0.3, alpha: 1 }
back_color = theme_color
slider_color = theme_color.merge({ red: theme_color[:red] - 0.1, green: theme_color[:green] - 0.1, blue: theme_color[:blue] - 0.1 })
color_label =  theme_color.merge({ red: theme_color[:red] + 0.6, green: theme_color[:green] + 0.6, blue: theme_color[:blue] + 0.6 })
s = slider({ icon: :help,
             back: { atome_id: :help,
                     color: back_color,
                     shadow: { bounding: true }, smooth: '6%', size: 66 },
             max: 333,
             duration: 0,
             orientation: :vertical,
             cursor: { value: 0.3,
                       range: [0, 450],
                       unit: :freq,
                       back_color: slider_color,
                       color: color_label },
             label: { color: color_label, visual: 12, center: :x }

           })
# s = slider({ icon: :help,
#              back: { atome_id: :help,
#                      color: { red: 1, blue: 0, green: 0.6, alpha: 1 },
#                      shadow: { bounding: true }, smooth: '6%', size: 66 },
#              max: 333,
#              duration: 0.3,
#              orientation: :vertical,
#              cursor: { value: 0.3,
#                        range: [0, 450],
#                        unit: :freq,
#                        back_color: { red: 0.6, blue: 0.3, green: 0, alpha: 0.3 },
#                        color: { red: 0.9, blue: 0.9, green: 0.9, alpha: 1 } },
#              label: { color: { red: 0.9, blue: 0.9, green: 0.9, alpha: 1 }, visual: 12, center: :x }
#
#            })
s.y(333)
s.x(s.y)