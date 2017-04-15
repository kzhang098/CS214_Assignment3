
int netopen(const char *path, int oflags) {
	return -1 * open(path, oflags);
}

ssize_t netread(int fildes, void *buf, size_t nbyte) {
	return read(-1 * fildes, buf, nbyte);
}

ssize_t netwrite(int fildes, const void *buf, size_t nbyte) {
	return write(-1 * fildes, buf, nbyte);
}

 int netclose(int fd) {
	 return close(-1 * fd);
 }