echo "=== Removing existing Modules ==="
rmmod control_pipe datarecorder rec_ringbuffer rec_state || true
echo "=== Inserting Modules ==="
modprobe datarecorder
modprobe control_pipe
