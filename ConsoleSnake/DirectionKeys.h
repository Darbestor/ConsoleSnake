#define ESC "\x1b"
#define CSI "\x1b["

static struct DirectionKeys
{
	static constexpr char UP[] = CSI "A";
	static constexpr char DOWN[] = CSI "B";
	static constexpr char LEFT[] = CSI "D";
	static constexpr char RIGHT[] = CSI "C";
};