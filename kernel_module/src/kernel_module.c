#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <asm/io.h>

// Informacje o module
MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jakub Strzelczyk");
MODULE_DESCRIPTION("Simple kernel module for SYKOM project");
MODULE_VERSION("0.01");

// Definicje konfiguracyjne dla zarządzania przestrzenią GPIO i procesami kończącymi moduł
#define SYKT_GPIO_BASE_ADDR  (0x00100000)
#define SYKT_GPIO_SIZE       (0x8000)
#define SYKT_EXIT            (0x3333)
#define SYKT_EXIT_CODE       (0x7F)

// Definicje adresów dla różnych rejestrów w przestrzeni GPIO, wykorzystują bazowy adres
void __iomem *baseptr;
#define SYKT_GPIO_ADDR_SPACE (baseptr)
#define REG_A (SYKT_GPIO_ADDR_SPACE + 0x224)
#define REG_S (SYKT_GPIO_ADDR_SPACE + 0x23C)
#define REG_W (SYKT_GPIO_ADDR_SPACE + 0x234)

// Deklaracja zmiennych
static struct proc_dir_entry *proc_dir;
static unsigned int regAValue, regSValue, regWValue;

// Funkcje zapisu wartości do rejestrów
static ssize_t regA_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    if (copy_from_user(buf, buffer, min(count, sizeof(buf) - 1)))
        return -EFAULT;
    buf[min(count, sizeof(buf) - 1)] = '\0';
    if (kstrtouint(buf, 8, &regAValue))
        return -EINVAL;
    writel(regAValue, REG_A);
    return count;
}

static ssize_t regS_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    if (copy_from_user(buf, buffer, min(count, sizeof(buf) - 1)))
        return -EFAULT;
    buf[min(count, sizeof(buf) - 1)] = '\0';
    if (kstrtouint(buf, 8, &regSValue))
        return -EINVAL;
    writel(regSValue, REG_S);
    return count;
}

static ssize_t regW_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    if (copy_from_user(buf, buffer, min(count, sizeof(buf) - 1)))
        return -EFAULT;
    buf[min(count, sizeof(buf) - 1)] = '\0';
    if (kstrtouint(buf, 8, &regWValue))
        return -EINVAL;
    writel(regWValue, REG_W);
    return count;
}

// Funkcje odczytu wartości z rejestrów
static ssize_t regA_read(struct file *file, char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    regAValue = readl(REG_A);
    int len = snprintf(buf, sizeof(buf), "%o\n", regAValue);
    if (*pos >= len)
        return 0;
    if (copy_to_user(buffer, buf, len))
        return -EFAULT;
    *pos += len;
    return len;
}

static ssize_t regS_read(struct file *file, char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    regSValue = readl(REG_S);
    int len = snprintf(buf, sizeof(buf), "%o\n", regSValue);
    if (*pos >= len)
        return 0;
    if (copy_to_user(buffer, buf, len))
        return -EFAULT;
    *pos += len;
    return len;
}

static ssize_t regW_read(struct file *file, char __user *buffer, size_t count, loff_t *pos)
{
    char buf[13];
    regWValue = readl(REG_W);
    int len = snprintf(buf, sizeof(buf), "%o\n", regWValue);
    if (*pos >= len)
        return 0;
    if (copy_to_user(buffer, buf, len))
        return -EFAULT;
    *pos += len;
    return len;
}

// Definicje interfejsów plików w /proc
static const struct file_operations proc_regA_ops = {
    .owner = THIS_MODULE,
    .read = regA_read,
    .write = regA_write,
};

static const struct file_operations proc_regS_ops = {
    .owner = THIS_MODULE,
    .read = regS_read,
    .write = regS_write,
};

static const struct file_operations proc_regW_ops = {
    .owner = THIS_MODULE,
    .read = regW_read,
    .write = regW_write,
};

// Inicjalizacja modułu
static int __init my_init_module(void)
{
    printk(KERN_INFO "Init my module.\n");
    baseptr = ioremap(SYKT_GPIO_BASE_ADDR, SYKT_GPIO_SIZE);
    if (!baseptr) {
        printk(KERN_ERR "Error while mapping base address.\n");
        return -ENOMEM;
    }

    proc_dir = proc_mkdir("sykom", NULL);
    if (!proc_dir) {
        printk(KERN_ERR "Error while creating /proc/sykom directory.\n");
        iounmap(baseptr);
        return -ENOMEM;
    }

    if (!proc_create("rejstrjakA", 0644, proc_dir, &proc_regA_ops) ||
        !proc_create("rejstrjakS", 0644, proc_dir, &proc_regS_ops) ||
        !proc_create("rejstrjakW", 0444, proc_dir, &proc_regW_ops)) {
        printk(KERN_ERR "Cannot create /proc/sykom entries.\n");
        remove_proc_entry("rejstrjakA", proc_dir);
        remove_proc_entry("rejstrjakS", proc_dir);
        remove_proc_entry("rejstrjakW", proc_dir);
        remove_proc_entry("sykom", NULL);
        iounmap(baseptr);
        return -ENOMEM;
    }

    return 0;
}

// Czyszczenie modułu
static void __exit my_cleanup_module(void)
{
    printk(KERN_INFO "Cleanup my module.\n");
    writel(SYKT_EXIT | ((SYKT_EXIT_CODE) << 16), baseptr);
    remove_proc_entry("rejstrjakA", proc_dir);
    remove_proc_entry("rejstrjakS", proc_dir);
    remove_proc_entry("rejstrjakW", proc_dir);
    remove_proc_entry("sykom", NULL);
    iounmap(baseptr);
}

module_init(my_init_module);
module_exit(my_cleanup_module);