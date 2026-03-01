#ifndef SHADER_HPP
#define SHADER_HPP

#include <core/core.hpp>
#include <cmath>

inline Vector3 compute_fragment_final(
	Vector3 normal, Vector3 diffuse, Vector3 glow, Vector3 light_direction, Vector3 view_direction, f32 specular_val, f32 shininess
) {

	f32 ambient_intensity = 0.2f;

	Vector3 result{};

	// ambient
	Vector3 ambient = diffuse * ambient_intensity;

	// diffuse 
	// make sure light direction is already normalized
	f32 diffuse_strength = Math::maximum(Math::dot(normal, -light_direction), 0.0f);
	diffuse = diffuse * diffuse_strength;

	// specular
	Vector3 reflection = Math::reflect(light_direction, normal);
	f32 specular_strength = shininess ? pow(Math::maximum(Math::dot(view_direction, reflection), 0.0f), shininess) : 0;
	Vector3 specular = Vector3{ specular_val, specular_val, specular_val } * specular_strength;

	result += ambient + diffuse + specular + glow;

	return result;
}

inline Vector3 compute_fragment(Vector3 normal, Vector3 light_direction, Vector3 view_direction, Vector3 color, f32 shininess) {

	f32 light_ambient = 0.2f;
	f32 light_diffuse = 0.6f;
	f32 light_specular = 0.4f;

	Vector3 result{};

	// ambient
	Vector3 ambient = color * light_ambient;

	// diffuse 
	// make sure light direction is already normalized
	f32 diffuse_strength = Math::maximum(Math::dot(normal, -light_direction), 0.0f);
	Vector3 diffuse = color * light_diffuse * diffuse_strength;

	// specular
	Vector3 reflection = Math::reflect(light_direction, normal);
	f32 specular_strength = shininess ? pow(Math::maximum(Math::dot(view_direction, reflection), 0.0f), shininess) : 0;
	Vector3 specular = color * light_specular * specular_strength;

	result += ambient + diffuse + specular;

	return result;
}

#endif // SHADER_HPP
