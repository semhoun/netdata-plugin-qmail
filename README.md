# qmail.plugin

`qmail.plugin` is a netdata external plugin. It detects **qmail** presence by checking `/var/log/qmail` directory existence and there it locates all subdirectories containing `smtp` or `send` in theirs name and prepares data collector for each one of them.

It skips all directories starting with `.` character.

**For smtp it collects**:

1. connection with status `ok` or `deny`,
2. average number of connections,
3. end statuses for `0`, `256`, `25600` or *other* value,
4. connection via SMTP protocol type `SMTP` or `ESMTPS`,
5. usage of TLS protocol version `TLS1`, `TLS_1`, `TLS_1.1`, `TLS_1.2`, `TLS_1.3` or *unknown*.

**For send it collects**:

1. number of `start delivery` and `end msg`,
2. number of delivery `success`, `failure` or `deferral`.

This plugin is currently Linux specific.

# scanner.plugin

`scanner.plugin` is a netdata external plugin. It detects **scannerd** presence by checking `/var/log` directory existence (Well, well, this is obviously incorrect) and there it locates all subdirectories containing `scannerd` in theirs name and prepares data collector for each one of them.

It skips all directories starting with `.` character.

**It collects**:

1. Emails with status `Clear`, `CLAMDSCAN`, `SPAM-TAGGED`, `SPAM-REJECTED` and `SPAM-DELETED`
2. Spam Cache hits
3. Antivirus Cache hits

This plugin is currently Linux specific.

# svstat.plugin

`svstat.plugin` is a netdata external plugin. It detects presence of a [daemontools](http://cr.yp.to/daemontools.html) `/service` directory and enumerates `uptime`/`downtime` of all individual services. The plugin collects uptime or downtime in, respectively, positive or negative number of seconds since last change of the service.

The plugin skips all subdirectories starting with `.` character.

### Plugin restart

It is possible to restart service by sending signal `QUIT`, `TERM` or `INT` (with command `pkill qmail.plugin` for example) and `qmail.plugin` quits successfully
It will be started by `netdata` again.
This may be wanted if the plugin have been updated or new log directory have been introduced.
