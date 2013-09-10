local M = Class(DisplayObject)

function M:init(normal, active)
	DisplayObject.init(self)

	self.normal = normal
	self.active = active
	self.focus = nil

	self:addChild(self.normal)
	self:addChild(self.active)
	self:updateVisualState(false)

	self:addEventListener(Event.MOUSE_DOWN, self.onMouseDown, self)
	self:addEventListener(Event.MOUSE_MOVE, self.onMouseMove, self)
	self:addEventListener(Event.MOUSE_UP, self.onMouseUp, self)

	self:addEventListener(Event.TOUCHES_BEGIN, self.onTouchesBegin, self)
	self:addEventListener(Event.TOUCHES_MOVE, self.onTouchesMove, self)
	self:addEventListener(Event.TOUCHES_END, self.onTouchesEnd, self)
	self:addEventListener(Event.TOUCHES_CANCEL, self.onTouchesCancel, self)
end

function M:onMouseDown(e)
	if self.focus == nil and self:hitTestPoint(e.info.x, e.info.y) then
		self.focus = 0
		self:updateVisualState(true)
		e:stopPropagation()
	end
end

function M:onMouseMove(e)
	if self.focus == 0 then
		if not self:hitTestPoint(e.info.x, e.info.y) then	
			self.focus = nil
			self:updateVisualState(false)
		end
		e:stopPropagation()
	end
end

function M:onMouseUp(e)
	if self.focus == 0 then
		self.focus = nil
		self:updateVisualState(false)
		self:dispatchEvent(Event:new("Click"))
		e:stopPropagation()
	end
end

function M:onTouchesBegin(e)
	if self.focus == nil and self:hitTestPoint(e.info.x, e.info.y) then
		self.focus = e.info.id
		self:updateVisualState(true)
		e:stopPropagation()
	end
end

function M:onTouchesMove(e)
	if self.focus == e.info.id then
		if not self:hitTestPoint(e.info.x, e.info.y) then
			self.focus = nil
			self:updateVisualState(false)
		end
		e:stopPropagation()
	end
end

function M:onTouchesEnd(e)
	if self.focus == e.info.id then
		self.focus = nil
		self:updateVisualState(false)
		self:dispatchEvent(Event:new("Click"))
		e:stopPropagation()
	end
end

function M:onTouchesCancel(e)
	if self.focus == e.info.id then
		self.focus = nil;
		self:updateVisualState(false)
		e:stopPropagation()
	end
end

function M:updateVisualState(state)
	if state then
		self.normal:visible(false)
		self.active:visible(true)
	else
		self.normal:visible(true)
		self.active:visible(false)
	end
end

return M
