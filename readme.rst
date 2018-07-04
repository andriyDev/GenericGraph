OFSM
==================

This is an open source, finite state machine (FSM) system for Unreal. The original GenericGraph project from which this has forked was flawed in that it could not support cycles (a relatively common thing for finite state machines).

## Usage

The first step is to create a finite state machine. Right click on your content browser and select OFSM > OFSM. Opening this asset will open an empty graph.

Begin by adding nodes. If you select these nodes, you can change their Identifier (what you use to search for it at runtime), and its Node Title (what you see in the graph editor).
Next, connect nodes together by dragging from the top of one node to the bottom of another (or vice versa). This will create a transition in the direction of the arrow. Selecting the circle next to the arrow will let you change the Identifier (what you use to refer to this transition). You may also wish to change the Transition Priority. If there are multiple transitions possible, the one with the highest priority will be selected.
Left clicking anywhere but nodes or transitions will show the FSM properties. Here you can specify the initial state (the Identifier of the node), as well as the handler class.

A handler binds an OFSMComponent to events to simplify interaction with the FSM.
Create a handler by creating a new Blueprint Class, and under "All Classes", selecting "OFSMHandler"
Inside the handler, override the "Bind FSM" function. From here, you can call "Add Binding", which requires the OFSMComponent (this is provided) and the Identifier of the transition you wish to bind (Note: Identifiers for transitions do not need to be unique. If two transitions have the same requirements for validity, you may use the same Identifier to share the behaviour). This returns an object with an event dispatcher "Validity". You can bind an event to this. In this event, it is crucial that you call "Return Value" on Source, and provide a boolean value which corresponds to whether the transition can be taken.
Do not forget to add this object in your FSM's properties.

To actually use your FSM, you require an actor. Add an OFSM Component to this actor. Assign your FSM to the component's FSM property. You may also change the Default Transition State which says how to treat transitions that do not have a validity binding.
Call "Initialize FSM" on your component before using it (during BeginPlay is a good idea).
After this, you may call "Update State" to have the component change states (if possible).
You can get and set the current state directly from Blueprints. This allows you to change behaviour based on the current state.
You can also bind an event to the component's OnStateChange event dispatcher. This will notify you just before a transition occurs, so that you can clean up the previous state, and then setup the new state before it is assigned (this is handled for you).

The final major piece is adding custom node types/edge types. You can create child blueprints of OFSMNode and OFSMEdge. This allows you to store more information in each bit, allowing you to extend the FSM system further. For example, adding a text property to nodes can be an NPC's text, and then another text property can be a player's response leading to another node.

It is also important to mention that using "Update State" is optional. You can make your own state updater. For example, in a dialogue system, the valid transitions could be responses that the player could choose from, but the state should switch to the node that corresponds to the response the player selects. The functions "Get First Valid Transition" and "Get Valid Transitions" are utilities that can help with this.

## Credits

Major thanks to jinyuliao for the base of this plugin.
