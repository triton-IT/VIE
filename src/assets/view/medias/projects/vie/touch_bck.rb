b = box({ atome_id: :my_box_id })

# def poilu(ev,proc)
#   instance_exec ev, &proc if proc.is_a?(Proc)
# end

# ######## immediate drag works!!
# def touchy(ev,proc)
#   puts "hit "
#   # instance_exec ev, &proc if proc.is_a?(Proc)
# end

# def poilu(&proc)
#   Element.find("##{atome_id}").on('mousedown') do |evt|
#     # proc.call(evt)
#     native_ev = evt.to_n
#     `
#   $(#{native_ev}.target).draggable();
#    $(#{native_ev}.target).trigger(#{native_ev});
# `
#     # instance_exec evt, &proc if proc.is_a?(Proc)
#     # alert evt.to_n
#   end
#
# end

# b.poilu({ option: :down} ) do |ev|
#
#   # puts ev.type
#   # puts ev.element
#   # puts ev.to_n
#
#
# end
# ######## immediate drag works!!

# def opal_touch(params, ev, proc, current_atome)
#   current_atome.instance_exec ev, &proc if proc.is_a?(Proc)
# end

# class Atome
#
#   def touch(params, &proc)
#
#     if params == :remove
#
#     end
#     case params.keys[0]
#     when :long
#       # rurrent_id= atome_id
#       time = params[:long][:time] if params[:long].instance_of?(Hash)
#       time ||= 1
#
#       `
#       var timer;
# $('#'+#{atome_id}).on("touchstart mousedown",function(ev){
#   timer = setTimeout(function(){
#     Opal.Atome.$opal_touch(#{params},ev,#{proc}, #{self})
#     },1000*#{time});
# }).on("mouseup mouseleave",function(ev){
#    clearTimeout(timer);
#
# })
# `
#
#     when :down
#
#     when :up
#       `
# $('#'+#{atome_id}).on("touchend mouseup",function(ev){
#     Opal.Atome.$opal_touch(#{params},ev,#{proc}, #{self})
# })
# `
#     when :click
#       `
# $('#'+#{atome_id}).on("touchend mouseup",function(ev){
#     Opal.Atome.$opal_touch(#{params},ev,#{proc}, #{self})
# })
# `
#     else
#
#     end
#
#     self
#   end
# end

b.touch ({ option: :long }) do |ev|
  b.drag(true)
  #   `
  # //$(#{ev}.target).draggable();
  #    $(#{ev}.target).trigger(#{ev});
  # `
  virtual_event({ event: ev.to_n})

  puts self.atome_id

  b.color(:red)
end



b.touch ({ up: true }) do |ev|
  b.color(:blue)
  b.drag(:destroy)

end