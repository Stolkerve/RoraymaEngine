import PyScripting
from PyScripting import Math, Input

#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.

class Sprite(PyScripting.PyScript):
	transform = None
	def OnStart(self):
		self.transform = self.entity.GetTransformComponent()
	def OnUpdate(self, _delta):
		self.transform.position.x += 0.1
		pass
	def OnQuit(self):
		pass




