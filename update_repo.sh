echo "===Cleaning workspace==="
make clean -C xenomai_cokernelmodule_datarecorder/
echo "===Adding new files==="
git add -A
echo "===Commiting==="
git commit
git push
