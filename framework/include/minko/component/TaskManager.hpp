/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "minko/Common.hpp"

#include "minko/component/AbstractScript.hpp"
#include "minko/Signal.hpp"

namespace minko
{
	namespace component
	{
		class TaskManager :
			public AbstractScript
		{

		public:
			
			class Task
			{
			public :
				typedef std::shared_ptr<Task> Ptr;

				friend TaskManager;

			protected :
				std::shared_ptr<TaskManager>	_taskManager;
				bool							_running;
				
			public:

				virtual
				bool
				complete() = 0;

				virtual
				void
				beforeFirstStep() = 0;

				virtual
				void
				step() = 0;
				
				virtual
				float
				priority() = 0;
				
				virtual
				void
				afterLastStep() = 0;

				inline
				bool
				running()
				{
					return _running;
				}

				inline
				void
				running(bool value)
				{
					_running = value;
				}

				inline
				std::shared_ptr<TaskManager>
				taskManager()
				{
					return _taskManager;
				}
			protected :
				Task();
			};

		public:
			typedef std::shared_ptr<TaskManager>	Ptr;

		private:
			typedef std::shared_ptr<scene::Node> NodePtr;
		
		private:
			unsigned int			_loadingFramerate;
			float					_frameTime;
			std::vector<Task::Ptr>	_tasks;
			clock_t					_frameStartTime;

		public:
			static
			Ptr
			create(unsigned int loadingFramerate)
			{
				Ptr taskManager(new TaskManager(loadingFramerate));

				taskManager->initialize();

				return taskManager;
			};

			Ptr
			pushTask(Task::Ptr task);

			void
			update(NodePtr target);

			void
			end(NodePtr target);

		private:
			TaskManager(unsigned int loadingFramerate);
		};
	}
}