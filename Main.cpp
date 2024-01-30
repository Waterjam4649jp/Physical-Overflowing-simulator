# include <Siv3D.hpp>
# include <iostream>

struct PhysicsCircle
{

	/* --変数--
	*  pos_0 ... 初期位置
	*  pos   ... 現在位置(tに依存)
	*  v_0   ... 初速度
	*  v     ... 現在速度(tに依存)
	*  a     ... 加速度(gは随時引数に追加)
	*  r     ... 半径
	*  m     ... 質量
	*  theta ... 角度(度数法)
	*  omega ... 角速度
	*  I     ... 慣性モーメント
	* 
	*  delta_t ... 単位時間(基本は 1s / fps)
	*/

	Vec2 pos_0, v_0, a, pos, v;
	double r, m, delta_t;
	double theta, omega, I;
	LineString LineString;
	PhysicsCircle() = default;

	PhysicsCircle(const Vec2& position,
				  const double radius,
				  const double Mass = 1,
				  const double delta_time = Scene::DeltaTime(),
				  const Vec2& velocity = {},
				  const Vec2& acceralation = {},
				  const double rad_velocity = {})
	{
		pos_0 = position;
		pos = position;
		v_0 = velocity;
		v = velocity;
		a = acceralation;
		r = radius;
		m = Mass;
		delta_t = delta_time; // 1フレーム毎に何秒経っているかを基に計算
		theta = 0;
		omega = rad_velocity;
		I = 0.5 * m * r * r;
		LineString;
	}

	// ----物理演算----

	void update()
	{
		theta = (theta < 0 || theta >= 360 ? theta = fmod(theta, 360) : theta);

		v += (a * delta_t);
		pos += (v * delta_t);
		theta += omega * delta_t;
	}

	//重心を基準とした相対座標から力積を加える。
	void addImpulseLocal(const Vec2& impulse, const Vec2& addLocalPos)
	{
		v += impulse / m;
		omega += addLocalPos.cross(impulse) / I;
	}

	//絶対座標から力積を加える。
	void addImpulse(const Vec2& impulse, const Vec2& addPos)
	{
		addImpulseLocal(impulse, addPos - pos);
	}

	// ----描画関係----

	const Circle circle()
	{
		return {pos, r};
	}

	const Line delta_line()
	{
		return { pos, pos - (v * delta_t) }; // 二点目の座標はdelta_t秒前の座標
	}

	void draw(const ColorF& color_C = Palette::White, const int Thickness = 0, const ColorF color_L = Palette::Red)
	{
		circle().draw(color_C);
		Line{ pos, Arg::direction = Circular(r,theta).toVec2() }.draw(Thickness, color_L);
	}

	void trajectory(const int Thickness = 0,  const ColorF color_L = Palette::Red) // 太さを入力すると線が描画される
	{
		LineString << ( pos, pos - (v * delta_t) );
		LineString.draw(Thickness, color_L);
	}

};

void Main()
{
	Window::Resize(1600, 900);
	Window::SetStyle(WindowStyle::Sizable);

	const double delta_t = 0; // (1s / 60fps) = 0.05
	double t = 0;

	Vec2 g = {0, 9.8 * 20};
	PhysicsCircle o{Vec2(200, 500), 60, 1, delta_t, Vec2(100, -400), g};
	PhysicsCircle o2{ Scene::CenterF() + Vec2(400, 0), 60, 1, delta_t, Vec2(0, 0), g};

	while (System::Update())
	{
		if (KeySpace.down())
		{
			o.delta_t = 0.05;
			o2.delta_t = 0.05;
			o2.addImpulseLocal(Vec2(-50, -300), Vec2(0, 60));
		}
		ClearPrint();
		o.update();
		o2.update();
		o.draw(ColorF{ 0.5 });
		o2.draw(ColorF{ 0.1, 0.6, 0.6 }, 1, Palette::Red);
		o.trajectory(1);
		o2.trajectory(1, Palette::Green);
		t += delta_t;
		Print << t;
		Print << o.v;
	}
}
