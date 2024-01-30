# include <Siv3D.hpp>
# include <iostream>

struct PhysicsCircle
{
	Vec2 pos_0, v_0, a, pos, v;
	double r, m, delta_t;
	Vec2 P;
	double theta, I;
	LineString LineString;
	PhysicsCircle() = default;

	PhysicsCircle(const Vec2& position,
				  const double radius,
				  const double Mass,
				  const double delta_time,
				  const Vec2& velocity = {},
				  const Vec2& acceralation = {}	)
	{
		pos_0 = position;
		v_0 = velocity;
		a = acceralation;
		r = radius;
		m = Mass;
		delta_t = delta_time; // 1フレーム毎に何秒経っているかを基に計算
		theta = 0;
		LineString;
	}

	// ----物理演算----

	void update()
	{
		theta = (theta < 0 || theta >= 360 ? theta = fmod(theta, 360) : theta);

		v_0 += (a * delta_t);
		v = v_0;
		pos_0 += (v * delta_t);
		pos = pos_0;
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
	//double t;

	Vec2 g = {0, 9.8 * 20};
	PhysicsCircle o{Vec2(200, 500), 60, 1, delta_t, Vec2(100, -400), g};
	PhysicsCircle o2{ Scene::CenterF(), 60, 1, delta_t, Vec2(0, 0), Vec2(0, 0)};

	while (System::Update())
	{
		if (KeySpace.down())
		{
			o.delta_t = 0.05;
			o2.delta_t = 0.05;
		}
		ClearPrint();
		o.update();
		o2.update();
		o.draw(ColorF{ 0.5 });
		o2.draw(ColorF{ 0.1, 0.6, 0.6 }, 1, Palette::Red);
		o.trajectory(1);
		o2.theta += 0.1;
		t += delta_t;
		Print << t;
		Print << o.v;
		Print << o2.theta;
	}
}
