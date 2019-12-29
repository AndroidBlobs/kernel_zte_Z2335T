#include <linux/proc_fs.h>
#include "tpd_sys.h"
#include "cts_platform.h"
#include "cts_core.h"

static int tpd_init_tpinfo(struct tpd_classdev_t *cdev)
{
	struct cts_device *cts_dev;
	int ret = 0;

	cts_dev = (struct cts_device *)cdev->private;
	if (cts_dev->rtdata.suspended) {
		cts_err("cts tp in suspned");
		return -EIO;
	}

	cts_lock_device(cts_dev);
	ret = cts_get_firmware_version(cts_dev, &cts_dev->fwdata.version);
	if (ret) {
		cts_err("Read firmware version failed %d", ret);
		cts_unlock_device(cts_dev);
		return ret;
	}
	cts_unlock_device(cts_dev);

	snprintf(cdev->ic_tpinfo.tp_name, sizeof(cdev->ic_tpinfo.tp_name), "chipone");
	cdev->ic_tpinfo.module_id = cts_dev->confdata.hw_sensor_id;

	if (cdev->ic_tpinfo.module_id == CTS_VENDOR_ID0) {
		strlcpy(cdev->ic_tpinfo.vendor_name, CTS_VENDOR_NAME0, sizeof(cdev->ic_tpinfo.vendor_name));
	} else if (cdev->ic_tpinfo.module_id == CTS_VENDOR_ID1) {
		strlcpy(cdev->ic_tpinfo.vendor_name, CTS_VENDOR_NAME1, sizeof(cdev->ic_tpinfo.vendor_name));
	} else if (cdev->ic_tpinfo.module_id == CTS_VENDOR_ID2) {
		strlcpy(cdev->ic_tpinfo.vendor_name, CTS_VENDOR_NAME2, sizeof(cdev->ic_tpinfo.vendor_name));
	} else if (cdev->ic_tpinfo.module_id == CTS_VENDOR_ID3) {
		strlcpy(cdev->ic_tpinfo.vendor_name, CTS_VENDOR_NAME3, sizeof(cdev->ic_tpinfo.vendor_name));
	} else {
		strlcpy(cdev->ic_tpinfo.vendor_name, "unknown", sizeof(cdev->ic_tpinfo.vendor_name));
	}

	cdev->ic_tpinfo.chip_model_id = TS_CHIP_CHIPONE;
	cdev->ic_tpinfo.chip_ver = 0;
	cdev->ic_tpinfo.firmware_ver = cts_dev->fwdata.version;
	cdev->ic_tpinfo.i2c_type = 0;
	cdev->ic_tpinfo.i2c_addr = cts_dev->rtdata.i2c_addr;
	return 0;
}

static int tpd_register_fw_class(struct cts_device *cts_dev)
{
	tpd_fw_cdev.private = (void *)cts_dev;
	tpd_fw_cdev.get_tpinfo = tpd_init_tpinfo;
	return 0;
}

static ssize_t tp_module_info_read(struct file *file,
	char __user *buffer, size_t count, loff_t *offset)
{
	ssize_t len = 0;
	uint8_t buffer_tpd[200];
	struct tpd_classdev_t *cdev = &tpd_fw_cdev;

	if (*offset != 0) {
		return 0;
	}

	if (cdev->get_tpinfo) {
		cdev->get_tpinfo(cdev);
	}

	len += snprintf(buffer_tpd + len, sizeof(buffer_tpd) - len, "TP module: %s(0x%x)\n",
			cdev->ic_tpinfo.vendor_name, cdev->ic_tpinfo.module_id);
	len += snprintf(buffer_tpd + len, sizeof(buffer_tpd) - len, "IC type : %s\n",
			cdev->ic_tpinfo.tp_name);
	len += snprintf(buffer_tpd + len, sizeof(buffer_tpd) - len, "I2C address: 0x%x\n",
			cdev->ic_tpinfo.i2c_addr);
	len += snprintf(buffer_tpd + len, sizeof(buffer_tpd) - len, "Firmware version : 0x%x\n",
			cdev->ic_tpinfo.firmware_ver);
	if (cdev->ic_tpinfo.config_ver)
		len += snprintf(buffer_tpd + len, sizeof(buffer_tpd) - len, "Config version:0x%x\n",
			cdev->ic_tpinfo.config_ver);
	return simple_read_from_buffer(buffer, count, offset, buffer_tpd, len);
}

static const struct file_operations proc_ops_tp_module_Info = {
	.owner = THIS_MODULE,
	.read = tp_module_info_read,
};

int cts_common_interface_register(struct cts_device *cts_dev)
{
	struct proc_dir_entry *dir;
	struct proc_dir_entry *proc_file;

	dir = proc_mkdir("touchscreen", NULL);
	if (dir == NULL) {
		cts_err("%s: mkdir touchscreen failed!\n",  __func__);
		return -ENOMEM;
	}

	proc_file = proc_create("ts_information", 0664, dir, &proc_ops_tp_module_Info);
	if (proc_file == NULL) {
		cts_err("proc_create ts_information failed!\n");
	}

	tpd_register_fw_class(cts_dev);

	return 0;
}

