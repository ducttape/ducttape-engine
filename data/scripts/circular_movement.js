var radius = 2;
var speed = 0.5; // rotations per second

function OnCreate() {
    print("Component name property: " + this.component.name);
    print("Node: " + this.component.node.name);
}

function OnDestroy() {
    print("Destroying circular_path script component.");
}

function OnUpdate(time_diff) {
    this.component.node.SetPosition(
            this.radius * Math.sin(TotalTime * 2 * Math.PI * this.speed),
            this.radius * Math.cos(TotalTime * 2 * Math.PI * this.speed),
            0);
}
