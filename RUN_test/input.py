
import os

trick_sys.sched.set_enable_freeze(True)
trick_sys.sched.set_freeze_command(True)

simControlPanel = trick.SimControlPanel()
trick.add_external_application(simControlPanel)

trick.real_time_enable()
trick.itimer_enable()

trick.exec_set_software_frame(0.025)

trick.exec_set_thread_process_type(1, trick.PROCESS_TYPE_ASYNC_CHILD)
trick.exec_set_thread_amf_cycle_time(1, .0)
