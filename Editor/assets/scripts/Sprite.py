import PyScripting
from PyScripting import Math, Input

#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.

class Sprite(PyScripting.PyScript):
	transform = None
	def OnStart(self):
		self.transform = self.entity.GetTransformComponent()
		self.transform.position.x = 0.0
		self.transform.scale.x = 50.0
		self.transform.scale.y = 50.0
	def OnUpdate(self, _delta):
		self.transform.position.x += 1.0
		pass
	def OnQuit(self):
		pass

