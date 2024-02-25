#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/kernel.h>
#include <linux/etherdevice.h>
#include <linux/ip.h>

#define ETH_P_IPLIR 0x1234 // unused value
#define IPPROTO_IPLIR 0x56 // unused value

struct iplir_message {
    u8 version;
    u32 timestamp;
    u32 sourceIdentifier;
    u32 destinationIdentifier;
    u32 sequenceNumber;
    u32 initValue;
    
	struct {
        u8 type;
        u8 length;
        u8 value[0];
    } fields[0];
    
	u8 payloadData[0];
    struct {
        u8 SL;
        u8 M;
        u8 T;
        u8 I;
        u8 L;
        u8 S;
        u8 R2;
        u8 nextHeader;
    } staffing;

    u32 ICV; 
    u32 transitIdentifier;
    u32 transitInitValue;
    u32 TICV;

} __attribute__((packed));

int iplir_handler(struct sk_buff *skb, struct net_device *dev,
				  struct packet_type *pt, struct net_device *orig_dev)
{
	// needs to be implemented
	return 0;
}

struct net_device *virtualNIC;

int virtualNIC_open(struct net_device *dev) {
	printk("virtualNIC_open called\n");
	netif_start_queue(dev);
	return 0;
}

int virtualNIC_release(struct net_device *dev) {
	printk("virtualNIC_release called\n");
	netif_stop_queue(dev);
	return 0;
}

int virtualNIC_xmit(struct sk_buff *skb, struct net_device *dev) {
	
	printk("virtualNIC_xmit function called...\n");
    	dev_kfree_skb(skb);
	return 0;
}

int virtualNIC_init(struct net_device *dev) {
	printk("virtualNIC device initialized\n");
	dev->mtu = 100;
	// put other dev stuff here
	return 0;
}

const struct net_device_ops my_netdev_ops = {
	.ndo_init = virtualNIC_init,
	.ndo_open = virtualNIC_open,
	.ndo_stop = virtualNIC_release,
	.ndo_start_xmit = virtualNIC_xmit,
};

static void virtual_setup(struct net_device *dev){
	dev->netdev_ops = &my_netdev_ops;
}

int iplir_init(void) {

	int result;
	virtualNIC = alloc_netdev(0, "ArmanNIC", NET_NAME_UNKNOWN, virtual_setup);
	if((result = register_netdev(virtualNIC))) {
		printk("virtualNIC: Error %d initalizing card ...", result);
		return result;
	}

	return 0;
}

void iplir_exit(void)
{
	printk("Cleaning Up the Module\n");
	unregister_netdev(virtualNIC);
}

module_init(iplir_init);
module_exit(iplir_exit);

MODULE_LICENSE("GPL");
