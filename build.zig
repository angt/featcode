const std = @import("std");

const targets: []const std.Target.Query = &.{
    .{ .cpu_arch = .aarch64, .os_tag = .macos },
    .{ .cpu_arch = .x86_64, .os_tag = .macos },
    .{ .cpu_arch = .aarch64, .os_tag = .linux, .abi = .musl },
    .{ .cpu_arch = .x86_64, .os_tag = .linux, .abi = .musl },
};

pub fn build(b: *std.Build) void {
    for (targets) |target| {
        const arch_name = @tagName(target.cpu_arch.?);
        const os_name = @tagName(target.os_tag.?);
        const exe = b.addExecutable(.{
            .name = b.fmt("{s}-{s}-featcode", .{
                arch_name,
                os_name,
            }),
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
