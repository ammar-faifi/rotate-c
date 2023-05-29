const std = @import("std");

const exe_name = "vr";

const cflags = .{
    "-W",
    "-Wall",
    "-Wextra",
    "-Wconversion",
    "-Wpedantic",
    "-Wwrite-strings",
    "-fno-exceptions",
    "-Wstrict-prototypes",
    "-Wno-missing-field-initializers",
};

fn findCSourceFilesInDir(b: *const std.Build, dirname: []const u8) ![]const []const u8 {
    var dir = try std.fs.cwd().openIterableDir(dirname, .{});
    defer dir.close();

    var walker = try dir.walk(b.allocator);
    defer walker.deinit();

    var files = std.ArrayList([]const u8).init(b.allocator);

    while (try walker.next()) |ent| {
        if (ent.kind != .File) continue;
        if (!std.mem.endsWith(u8, ent.basename, ".c")) continue;
        const full = try std.fs.path.join(b.allocator, &.{ dirname, ent.path });
        try files.append(full);
    }

    return files.items;
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = exe_name,
        .root_source_file = null,
        .target = target,
        .optimize = optimize,
    });
    var s = findCSourceFilesInDir(b, "src") catch unreachable;
    exe.addCSourceFiles(s, &cflags);
    exe.addIncludePath("src/inc/");
    exe.c_std = .C11;
    exe.linkLibC();
    // This declares intent for the executable to be installed into the
    // standard location when the user invokes the "install" step (the default
    // step when running `zig build`).
    b.installArtifact(exe);

    // This *creates* a RunStep in the build graph, to be executed when another
    // step is evaluated that depends on it. The next line below will establish
    // such a dependency.
    const run_cmd = b.addRunArtifact(exe);

    // By making the run step depend on the install step, it will be run from the
    // installation directory rather than directly from within the cache directory.
    // This is not necessary, however, if the application depends on other installed
    // files, this ensures they will be present and in the expected location.
    run_cmd.step.dependOn(b.getInstallStep());

    // This allows the user to pass arguments to the application in the build
    // command itself, like this: `zig build run -- arg1 arg2 etc`
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // This creates a build step. It will be visible in the `zig build --help` menu,
    // and can be selected like this: `zig build run`
    // This will evaluate the `run` step rather than the default, which is "install".
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    // Creates a step for unit testing. This only builds the test executable
    // but does not run it.
    const unit_tests = b.addTest(.{
        .root_source_file = .{ .path = "src/test.zig" },
        .target = target,
        .optimize = optimize,
    });

    const run_unit_tests = b.addRunArtifact(unit_tests);

    // Similar to creating the run step earlier, this exposes a `test` step to
    // the `zig build --help` menu, providing a way for the user to request
    // running the unit tests.
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_unit_tests.step);
}
