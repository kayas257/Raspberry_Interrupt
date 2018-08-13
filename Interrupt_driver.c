#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define GPIO_P3_9	4 
 
#define SW1_GPIO	26
#define NETLINK_USER 31 
struct sock *nl_sk = NULL;

int maj_num=0;
char buf[30];
int buflen=0;
static int my_dev_id=0, irq_counter = 0;
int pid;

static void __senduserdata(void)
{
   struct nlmsghdr *nlh;
   struct sk_buff *skb_out;
   int msg_size;
   int res;
   char *msg="Interupt received";
   msg_size=strlen(msg);
   skb_out = nlmsg_new(msg_size,0);
   if(!skb_out)
      {

      printk(KERN_ERR "Failed to allocate new skb\n");
      return;

   } 
   nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
   NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
   strncpy(nlmsg_data(nlh),msg,msg_size);
   res=nlmsg_unicast(nl_sk,skb_out,pid);
   if(res<0){
      printk(KERN_INFO "Error while sending bak to user\n");
      }
   static void hello_nl_recv_msg(struct sk_buff *skb) {
   struct nlmsghdr *nlh;
   struct sk_buff *skb_out;
   int msg_size;
   char *msg="Hello from kernel";
   int res;

   printk(KERN_INFO "Entering: %s\n", __FUNCTION__);
   msg_size=strlen(msg);
   nlh=(struct nlmsghdr*)skb->data;
   printk(KERN_INFO "Netlink received msg payload:%s\n",(char*)nlmsg_data(nlh));
   pid = nlh->nlmsg_pid; /*pid of sending process */

   skb_out = nlmsg_new(msg_size,0);

   if(!skb_out)
   {

      printk(KERN_ERR "Failed to allocate new skb\n");
      return;

   } 
   nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
   NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
   strncpy(nlmsg_data(nlh),msg,msg_size);
   res=nlmsg_unicast(nl_sk,skb_out,pid);
   if(res<0)
    printk(KERN_INFO "Error while sending bak to user\n");
  }




int led_init (int gpio_num)
{
  int retval = 0;
  retval = gpio_request(gpio_num,"hsled");
  if(retval < 0) 
  return retval;
  retval = gpio_direction_output(gpio_num,1);
  return retval;
}

void led_on(int gpio_num)
{
   gpio_set_value(gpio_num,1);
}

void led_off(int gpio_num)
{
  gpio_set_value(gpio_num,0);
}

void led_deinit(int gpio_num)
{
 gpio_free(gpio_num);
}

void obtacle(void)
{
  printk(KERN_ALERT "  Obstacle appeared ::::::- \n");
}
void led_toggle(void)
{
	static int status=0;
	
	if(status=(!status))
	{
	  led_on(GPIO_P3_9);
	  printk(KERN_ALERT " -:: LED ON ::- \n");
	}
	else
	{
	  led_off(GPIO_P3_9);
	   printk(KERN_ALERT " -:: LED OFF ::- \n");
	}
}


static irqreturn_t my_interrupt_handler(int irq, void *dev_id)
{
    irq_counter++;
    printk (KERN_INFO "In the ISR: counter = %d\n", irq_counter);
	led_init(GPIO_P3_9);    
//	led_toggle();
      __senduserdata();
       //obtacle();
    return IRQ_HANDLED;
}

static int __init my_init (void)
{

printk("Entering: %s\n",__FUNCTION__);
//This is for 3.6 kernels and above.
struct netlink_kernel_cfg cfg = {
    .input = hello_nl_recv_msg,
};
nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
if(!nl_sk)
{
    printk(KERN_ALERT "Error creating socket.\n");
    return -10;
}
	gpio_request(SW1_GPIO, "switch");
	gpio_direction_input(SW1_GPIO);
    	if (request_irq(gpio_to_irq(SW1_GPIO), my_interrupt_handler, IRQ_TYPE_EDGE_RISING, "my_interrupt", &my_dev_id))
        	return -1;


        printk (KERN_INFO "Successfully loading ISR handler\n");
    	return 0;
}

static void __exit my_exit (void)
{
    //synchronize_irq (irq);
    free_irq (gpio_to_irq(SW1_GPIO), &my_dev_id);
    led_deinit(GPIO_P3_9);
    printk (KERN_INFO "Successfully unloading \n");
}

module_init (my_init);
module_exit (my_exit);

MODULE_LICENSE("GPL");
