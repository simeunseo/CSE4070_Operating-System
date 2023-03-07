# CSE4175_Introduction-to-Computer-Network
서강대학교 운영체제(2022-2) pintOS 과제입니다.

## [Proj0-2] PintOS Data Structure
pintOS의 기본적인 자료구조를 구현한다.

## [Proj1] UserProgram(1)
- Argument Passing
사용자가 입력한 argument를 공백 단위로 분리하여 stack에 분리된 argument들과 argument의 갯수, argument의 주소 등 필요한 정보들을 올바르게 저장한다.
- User Memory Access
stack pointer가 null pointer이거나 kernel virtual memory, 또는 맵핑되지 않은 virtual memory에 접근하는 경우를 막고 잘못된 접근이 발생할 경우 exit(-1)을 호출한다.
- System Calls
user program에서 호출한 system call을 system call handler를 통해서, argument passing단계에서 stack에 저장된 값들을 이용해 적절한 함수를 호출함으로써 사용자가 원하는 작업을 처리한다.

## [Proj2] UserProgram(2)
- File Descriptor
파일 접근과 입출력을 위한 file descriptor를 구현한다.
- System Calls
close(파일을 닫음), read(파일의 데이터를 읽음), write(파일에 데이터를 기록함), seek(파일에서 원하는 위치로 이동함), tell(파일의 위치를 알려줌), remove(파일을 삭제함), create(파일을 생성함), filesize(파일의 크기를 알려줌)을 구현하여 적절히 실행되도록 한다.
- Synchronization in Filesystem
동시에 같은 파일에 접근하는 등 race condition이 발생할 수 있다. 이를 방지하기 위하여 lock을 잠금으로써 현재 사용 중인 파일에 다른 프로세스가 접근하지 못하도록 하고, 또 사용이 끝나면 적절한 때에 lock을 풀어준다.

## [Proj3] Threads
- Alarm Clock
기존의 timer_sleep() 함수에서는 thread가 running state와 ready state를 오가며 반복하게 하도록 함으로써 busy-waiting 방식으로 작동하고 있는데, 이는 cpu 자원 낭비를 초래하여 비효율적이므로 이 방식을 개선한다.
- Priority Scheduling
기존의 process 스케줄링 방식은 round-robin으로, thread간의 우선순위가 고려되지 않는다. 따라서 thread가 각자의 우선순위를 가지도록 하고 그 우선순위에 맞게 실행되게 하여 효율성을 높이고 starvation을 막기 위해 thread가 ready list에 머무르는 시간에 비례하여 우선순위를 높이는 aging을 구현한다.
- Advanced Scheduler
BSD scheduler에서는 각 우선순위가 각자의 ready queue를 가지는 multi level feedback queue를 이용하여 구현한다. 각 ready queue는 round-robin 방식으로 구현되며, 우선 순위가 높은 queue부터 스케줄링을 진행한다.
