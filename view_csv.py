import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("framerateV1.csv")

print("=== Framerate Data ===")
print(df.to_string(index=False))
print(f"\nTotal seconds: {len(df)}")
print(f"Average FPS: {df['fps'].mean():.2f}")
print(f"Average Fish Count: {df['fish_count'].mean():.2f}")

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

ax1.plot(df["time_second"], df["fps"], marker="o", color="b")
ax1.set_xlabel("Time (seconds)")
ax1.set_ylabel("FPS")
ax1.set_title("FPS over Time")
ax1.grid(True)

ax2.plot(df["time_second"], df["fish_count"], marker="o", color="g")
ax2.set_xlabel("Time (seconds)")
ax2.set_ylabel("Fish Count")
ax2.set_title("Fish Count over Time")
ax2.grid(True)

plt.tight_layout()
plt.show()