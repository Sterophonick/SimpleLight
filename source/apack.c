static const char *src;
static char *dest = 0, *destorg, lwm, mask, byte;
static unsigned int recentoff = 0, gamma;

static inline int getbit()
{
	if (mask&1)
		byte = *src++;
	mask = (mask>>1) | ((mask<<7) & (1<<7));
	return (byte&mask);
}

static inline void getbitgamma()
{
	gamma <<= 1;
	if (getbit())
		gamma++;
}

static inline void ap_getgamma()
{
	gamma = 1;
	do {
		getbitgamma();
	} while (getbit());
}

static inline void copyloop()
{
	do {
		*dest++ = dest[-recentoff];
	} while (--gamma);
}

static inline void ap_is_lwm()
{
	recentoff = *src++ + (gamma<<8);
	ap_getgamma();
	if (recentoff >= 32000)
		gamma++;
	if (recentoff >= 1280)
		gamma++;
	if (recentoff < 128)
		gamma += 2;
	copyloop();
}

int aP_depack(const char const *source, char *destination)
{
	int done = 0;
	src = source;
	dest = destination;
	*dest++ = *src++;
	mask = 0x01;
	lwm = 0;
	do {
		if (getbit()) {
			//apbranch1
			if (!getbit()) {
				//apbranch2
				ap_getgamma();
				gamma -= 2;
				if (lwm) {
					ap_is_lwm();
				} else {
					lwm = 1;
					if (gamma) {
						gamma--;
						ap_is_lwm();
					} else {
						ap_getgamma();
						copyloop();
					}
				}
			} else if (!getbit()) {
				//apbranch3
				gamma = *src++;
				recentoff = gamma>>1;
				if (!recentoff) {
					done = 1;
				} else {
					if (gamma&1) {
						*dest++ = dest[-recentoff];
					}
					*dest++ = dest[-recentoff];
					*dest++ = dest[-recentoff];
					lwm = 1;
				}
			} else {
				gamma = 0;
				getbitgamma();
				getbitgamma();
				getbitgamma();
				getbitgamma();
				if (gamma)
					gamma = dest[-gamma];
				*dest++ = (char) gamma;
				lwm = 0;
			}
		} else {
			*dest++ = *src++;
			lwm = 0;
		}
	} while(!done);
	return dest-destorg;
}