import os
os.system('git update-index --no-assume-unchanged imgui.ini')
os.system('git add imgui.ini')
os.system('git commit -m "🔧 [imgui.ini] Updated"')
os.system('git update-index --assume-unchanged imgui.ini')
