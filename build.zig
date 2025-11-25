const std = @import("std");

pub fn build(b: *std.Build) void {
    const cross = b.option(bool, "all", "Build for all supported targets") orelse false;

    const targets: []const std.Target.Query = if (cross) &.{
        .{ .cpu_arch = .aarch64, .os_tag = .macos },
        .{ .cpu_arch = .x86_64, .os_tag = .macos },
        .{ .cpu_arch = .aarch64, .os_tag = .linux, .abi = .musl },
        .{ .cpu_arch = .x86_64, .os_tag = .linux, .abi = .musl },
        .{ .cpu_arch = .aarch64, .os_tag = .freebsd },
        .{ .cpu_arch = .x86_64, .os_tag = .freebsd },
    } else &.{
        .{},
    };
    for (targets) |target| {
        const name = if (target.cpu_arch) |arch| b.fmt("{s}-{s}-featcode", .{
            @tagName(arch),
            @tagName(target.os_tag.?),
        }) else "featcode";

        const exe = b.addExecutable(.{
            .name = name,
            .root_module = b.createModule(.{
                .target = b.resolveTargetQuery(target),
                .optimize = .ReleaseSmall,
            }),
        });
        exe.addCSourceFiles(.{
            .files = &.{
                "featcode.c",
            },
        });
        exe.linkLibC();
        b.installArtifact(exe);
    }
}
