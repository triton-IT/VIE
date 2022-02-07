# How to use VIE

## Building the application

Create a build folder and get into it.

> mkdir build

> cd build

Create project structure

Option *-DDEBUG=TRUE* enable the logs.

Option *-DDEPLOY_TO_VST_FOLDER=TRUE* deploys VIE to the standard folder for virtual instruments.

> cmake .. -DDEPLOY_TO_VST_FOLDER=TRUE -DDEBUG=TRUE

Build the project and deploy it if parameter is set.

> cmake --build .

# Understanding VIE

## Input busses

### Event input busses

Midi processor component is mapped to an event input bus.
You need to add at list one midi processor component if you need to manage event inputs
Midi processor component is documented below.

### Audio input busses

Sampler processor component is mapped to an audio input bus.
You need to add at list one audio processor component if you want to manage audio inputs.
Sampler processor component is documented below.

## Output busses

### Event output busses

No event output bus is available for now.

### Audio output busses

Speaker processor component is mapped to an audio output bus.
You must have at least one speaker processor component to make VIE output communicates with VST host.
Speaker processor component is documented below.

## Components

Components goal is to transform input signals to output ones.
An output signal of an component must be bound to an input signal of another component of the same signal type.
For instance, the frequency output signal of the midi component is a float[32]. So it can be bound to the envelope 'frenquency' input signal which is also a float[32].

Standard components are listed below:

### Midi
Midi is an input component. It has no input signal but generate output ones.
Midi component mirrors in the processor the midi events sent by host.
The event sent to midi is enhanced with a unique id.
#### frequency
way: output
type: float\[32\]
#### velocity
way: output
type: float\[32\]
#### note active
way: output
type: uint32\[32\]
#### note on
way: output
type: note_event\[32\]
#### note off
way: output
type: note_event\[32\]

### Oscillator
Oscillator is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It generate a simple signal based on the chosen input type.
#### type => wave form
way: input
type: enum\[32\] (sin, saw, square)
Type of signal to generate in output slot.
#### frequency
way: input
type: float\[32\]
Frequency of the output signal to generate.
#### amplitudes
way: output
type: float\[32\]\[\<nb frames\>\]
Array of amplitudes generated from input type and input frequency.

### Envelope 
Envelope is a middle component. It consumes signal bound to is input slots and produces signal to its output ones.
It generate an output signal based on time, configured curves and events.
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
	- Process ùetho is called on each component.
	- Each component is requested to find zombie notes.
	- Each component is called to set zombie notes.

The 'set_input_values' of the (middle or output) component is called.
The 'set_zombie_notes_ids' of the component is called.
When component is ready to process (e.g. when all inputs are filled), the 'can_process' method must result 'true').
If component replied 'true' to 'can_process', then 'process'' method is called. Typically, the component behavior is launched and its output buffers are filled.
The 'get_output_values' method is then call and the resulting values are passed to the next linked components as input values.

When all components are processed, the orchestrator then call the 'has_finished' method of each component.
If the component has finished its job, it must return true. If it needs more frames, it must return 'false'.
When a component returns false to the 'has_finished' method, then, the 'get_zombie_notes_ids' method is called. The component must fill-in all the notes that are needed for further processing, including the 'potential' ones if needed.
If the notes whose processing is not finished are still alive, then they are sent on the next loop as normal notes, not zombie ones.
If a note is released and as been marked as 'zombie' by a component, then it is sent on the next loop as a 'zombie' note.
All components process this notes as a normal notes, but keep the 'zombie' tag on the note.
The component that did not finished the processing process the note but removes the 'zombie' tag.
If an output receives a note as 'zombie', then it must not render it.
This sequence with the 'zombie' note as to be battle tested, but should allow to handle logics like envelopes that must continue processing even if note is not played anymore.