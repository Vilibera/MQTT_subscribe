# This is mqtt_program Makefile
include $(TOPDIR)/rules.mk

PKG_NAME:=mqtt_program
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/mqtt_program
	CATEGORY:=Base system
	TITLE:=mqtt_program
	DEPENDS:=+libcurl +libmosquitto +libuci +libjson-c 
endef

define Package/mqtt_program/description
	This is a mqtt app package
endef

define Package/mqtt_program/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mqtt_program $(1)/usr/bin
	$(INSTALL_BIN) ./files/m_config.init $(1)/etc/init.d/m_conf
	$(INSTALL_CONF) ./files/m_config.config $(1)/etc/config/m_conf
endef

$(eval $(call BuildPackage,mqtt_program))
