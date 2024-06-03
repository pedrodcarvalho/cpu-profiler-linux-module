### 1. **Módulo do Kernel (Kernel Module)**

- **Headers**: `#include <linux/module.h>`, `#include <linux/kernel.h>`
- **Funções**: `module_init`, `module_exit`
- **Macros**: `MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION`, `MODULE_VERSION`

### 2. **Sistema de Arquivos `/proc`**

- **Headers**: `#include <linux/proc_fs.h>`, `#include <linux/seq_file.h>`
- **Funções**: `proc_create`, `proc_mkdir`, `remove_proc_entry`, `seq_printf`
- **Estruturas**: `proc_ops`, `proc_dir_entry`
- **Operações**: `log_proc_open`, `log_proc_show`, `procfile_write`, `seq_read`, `seq_lseek`, `single_open`, `single_release`

### 3. **Manipulação de Linked Lists**

- **Headers**: `#include <linux/list.h>`
- **Estruturas**: `list_head`, `list_entry`
- **Funções**: `list_add`, `list_for_each_safe`, `list_del`, `list_empty`, `INIT_LIST_HEAD`

### 4. **Gerenciamento de Memória**

- **Headers**: `#include <linux/slab.h>`
- **Funções**: `kmalloc`, `kfree`

### 5. **Timers**

- **Headers**: `#include <linux/timer.h>`, `#include <linux/jiffies.h>`
- **Funções**: `timer_setup`, `mod_timer`, `del_timer`
- **Estruturas**: `timer_list`
- **Macros**: `msecs_to_jiffies`, `jiffies`

### 6. **Workqueue (Fila de Trabalho)**

- **Headers**: `#include <linux/workqueue.h>`
- **Funções**: `create_workqueue`, `queue_work`, `destroy_workqueue`, `flush_workqueue`
- **Estruturas**: `workqueue_struct`, `work_struct`
- **Macros**: `INIT_WORK`

### 7. **Sincronização (Spinlocks)**

- **Headers**: `#include <linux/spinlock.h>`
- **Funções**: `spin_lock`, `spin_unlock`
- **Macros**: `DEFINE_SPINLOCK`

### 8. **Interação com Processos**

- **Headers**: `#include <linux/pid.h>`, `#include <linux/sched.h>`
- **Funções**: `find_task_by_pid`, `pid_task`, `find_vpid`
- **Estruturas**: `task_struct`

### 9. **Espaço de Usuário para Kernel**

- **Headers**: `#include <linux/uaccess.h>`
- **Funções**: `copy_from_user`

### 10. **RCU (Read-Copy Update)**

- **Funções**: `rcu_read_lock`, `rcu_read_unlock`

### 11. **Mensagens de Log**

- **Headers**: `#include <linux/printk.h>`
- **Funções**: `printk`
