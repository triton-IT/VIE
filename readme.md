#What is VIE?

VIE stands for Virtual Instrument Engine.
It means that when other VST are a single virtual instrument, VIE allows you create your own instrument or FX.

# How to use VIE

VIE is a plugin for any DAW supporting VST or AUV3.
To use VIE, you just have to download it for your platform/OS and follow the instruction of your DAW to install it.

Creating an instrument or FX is easy with VIE because of its design.
VIE interface is based on building blocks. You just have to drag blocks (called components) and link them to have a new instrument or FX ready to play.

VIE requires to link at least 2 types of building block, an input one and an output.
The signal will flow from your DAW to input components, flow through all intermediates ones and finish to flow to the output ones that will send result back to your DAW.

# Understanding VIE

## Input components

### Event input components

Midi-in component is mapped to an event input bus of your DAW. It means that if a midi-in component is present in VIE, it will handle the MIDI events of your DAW automatically.
You need to add at list one midi-in processor component if you need to manage event inputs.
Midi-in processor component is documented below.

### Audio input components

Audio-in component is mapped to an audio input bus of your DAW. It means that if a audio-in component is present in VIE, it will handle the AUDIO events of your DAW automatically.
You need to add at list one audio-in processor component if you want to manage audio inputs.
Audio-in processor component is documented below.

## Output busses

### Event output busses

No event output bus is available for now.

### Audio output busses

Audio-out component is mapped to an audio output bus. It means that if a audio-out component is present in VIE, the signals sent to the component will be redirect to your DAW.
You must have at least one speaker processor component to make VIE output communicates with VST host.
AUdio-out component is documented below.

## Components

Components goal is to transform input signals to output ones.
An output slot of an component must be bound to an input slot of another component.

*Standard components are listed below:*

### Midi-in
Midi-in is an input component.
Midi-in component mirrors the midi events sent by host.

#### Input slots:
##### on
Use this slot to enable/disable the component.
*values:* true means enabled, false means disabled.
*type:* boolean
*default:* true
#### Output slots:
##### frequency
This slot contains the frequencies of the midi notes currently played.
*values:* List of frequencies. One frequency by note currently pressed.
*type:* array of float.
*default:* empty
##### velocity
This slot contains the velocities of the midi notes currently played.
*values:* List of velocities. One velocity by note currently pressed.
*type:* array of float.
*default:* empty
##### note on
This slot contains the identifiers of the midi notes currently played.
*values:* List of identifiers. One identifier by note currently pressed.
*type:* No value.
*default:* empty
##### note off
This slot contains the identifiers of the midi notes previously playing but stopped.
*values:* List of identifiers. One identifier by note that just stop playing.
*type:* No value.
*default:* empty

### Oscillator
Oscillator is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It generate a simple signal based on the chosen input type.
#### type
This signal is a enum[32] which contains the type of the oscillator.
way: input
type: enum\[32\] (sin, saw, square)
Type of signal (wave form) to generate in output slot.
#### frequency
way: input
type: float\[32\]
Frequency of the output signal to generate.
#### amplitudes
way: output
type: float\[32\]\[\<nb frames\>\]

### Noise
Noise is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It generates an output noise signal based on the chosen input type.
#### On
way: input
type: bool
Must be on for noise to be generated.
#### amplitudes
way: output
type: float\[32\]\[\<nb frames\>\]
Array of amplitudes representing noise.

### Envelope 
Envelope is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It generates an output signal based on time, configured curves and events.
#### note on
way: input
type: note_event\[32\]
#### note off
way: input
type: note_event\[32\]
#### velocity
way: input
type: float\[32\]
#### amplitudes
way: output
type: float\[32\]\[\<nb frames\>\]

### Multiplier
Multiplier is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It multiplies all the operands by theirs sibling.
The number of resulting products is equals to the smallest number of values of multipliers and multiplicands.
#### multipliers
way: input
type: float\[32\]\[\<nb frames\>\]
#### multiplicands
way: input
type: float\[32\]\[\<nb frames\>\]
#### products
way: output
type: float\[32\]\[\<nb frames\>\]

### Mixer
Mixer is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It compute the average of all input values into a single output one.
#### generics
way: input
type: float\[32\]\[\<nb frames\>\]
#### average
way: output
type: float\[\<nb frames\>\]

### Output
Output is an output component. It consumes signal bound to is input slot and sends this input signal to the host.
#### generics
way: input
type: float\[32\]\[\<nb frames\>\]
#### average
way: output
type: float\[\<nb frames\>\]

### Sampler
Sampler is an input component. It has no input signal but generate output ones.
#### on
way: input
type: bool
#### frequency
way: output
type: float\[32\]
#### 

## Behavior of the processors components.

### dictionnary

- TODO: Review if useful. Potential note: A potential note is a note that is not processed by the component on this loop. But can be potentially used in another one.
For example, a 'conditional' component sends a a standard note on the output that satistfy condition and a 'potential' note to the output that do not satisfy it.
- Zombie note: A zombie note is a note that as been released, but that still need to be processed by some components.

### Interface to implement

All processor components (input, middle or output) must inherit from interface "processor_component".
Each instance of a component has its unique id.
Each component is of a given type (input, middle or output).

#### Behavior of components

- Events are propagated
- Processing method is performed.
	- Process method is called on each component.
	- Each component is requested to find zombie notes.
	- Each component is called to set zombie notes.

The 'set_input_values' of the (middle or output) component is called.
The 'set_zombie_notes_ids' of the component is called.
When component is ready to process (e.g. when all inputs are filled), the 'can_process' method must result 'true').
If component replied 'true' to 'can_process', then 'process' method is called. Typically, the component behavior is launched and its output buffers are filled.
The 'get_output_values' method is then call and the resulting values are passed to the next linked components as input values.

When all components are processed, the orchestrator then call the 'has_finished' method of each component.
If the component has finished its job, it must return true. If it needs more frames, it must return 'false'.
When a component returns false to the 'has_finished' method, then, the 'get_zombie_notes_ids' method is called. The component must fill-in all the notes that are needed for further processing, including the 'potential' ones if needed.
If the notes whose processing is not finished are still alive, then they are sent on the next loop as normal notes, not zombie ones.
If a note is released and as been marked as 'zombie' by a component, then it is sent on the next loop as a 'zombie' note.
All components process this notes as a normal notes, but keep the 'zombie' tag on the note.
The component that did not finished the processing on previous loop, processes the note but removes the 'zombie' tag.
If an output receives a note as 'zombie', then it must not render it.
This sequence with the 'zombie' note as to be battle tested, but should allow to handle logics like envelopes that must continue processing even if note is not played anymore.
