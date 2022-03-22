from setuptools import setup, find_packages

PyRolUI_pk_data = ["PyRolUI.pyd", "RolUI.dll"]
PyRolUI_glfw_pk_data = ["PyRolUI_glfw.pyd"]
PyRolUI_pk_data_stubs = ["__init__.pyi", "widgets.pyi", "signals.pyi"]
PyRolUI_glfw_pk_data_stubs = ["__init__.pyi"]

setup(
    name='PyRolUI',
    version='0.0.1',
    description='RolUI Python bind.',
    author='couc',
    python_requires='>=3.6',

    packages=["PyRolUI", "PyRolUI_glfw", "PyRolUI-stubs", "PyRolUI_glfw-stubs"],
    package_dir={
        "PyRolUI-stubs": "PyRolUI-stubs",
        "PyRolUI_glfw-stubs": "PyRolUI_glfw-stubs"
    },
    package_data={
        "PyRolUI": PyRolUI_pk_data, "PyRolUI_glfw": PyRolUI_glfw_pk_data,
        "PyRolUI-stubs": PyRolUI_pk_data_stubs,
        "PyRolUI_glfw-stubs": PyRolUI_glfw_pk_data_stubs
    }
)
