#version 450
layout(location = 0) out int materialOut;
layout(location = 1) out int instanceOut;

in flat int materialNumber;
in flat int instanceID;

void main() {
	materialOut = materialNumber;
	instanceOut = instanceID;
}
