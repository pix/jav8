package lu.flier.script;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.script.Bindings;

public class V8Object implements Bindings 
{
	protected long obj;
	
	public V8Object(long obj) {
		this.obj = obj;
	}

	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		
		this.internalRelease(this.obj);		
		
		this.obj = 0;
	}
	
	protected native void internalRelease(long ptr);
	private native String[] internalGetKeys();
	
	@Override
	public native int size();

	@Override
	public native boolean isEmpty();

	@Override
	public native void clear();

	@Override
	public native boolean containsKey(Object key);

	@Override
	public native Object get(Object key);

	@Override
	public native Object put(String name, Object value);

	@Override
	public native Object remove(Object key);

	@Override
	public Set<String> keySet() {
		Set<String> keys = new HashSet<String>();
		
		for (String key : internalGetKeys()) {
			keys.add(key);
		}
		
		return keys;
	}

	@Override
	public boolean containsValue(Object value) {
		for (String name : internalGetKeys()) {
			if (get(name) == value) {
				return true;
			}
		}
		return false;
	}

	@Override
	public Collection<Object> values() {
		Collection<Object> values = new ArrayList<Object>();
		
		for (String name : internalGetKeys()) {
			values.add(get(name));
		}
		
		return values;
	}

	@Override
	public Set<Map.Entry<String, Object>> entrySet() {
		Set<Map.Entry<String, Object>> entries = new HashSet<Map.Entry<String, Object>>();
		
		for (String name : internalGetKeys()) {
			entries.add(new HashMap.SimpleEntry<String, Object>(name, get(name)));
		}
		
		return entries;
	}

	@Override
	public void putAll(Map<? extends String, ? extends Object> toMerge) {
		for (Map.Entry<? extends String, ? extends Object> entry: toMerge.entrySet()) {
			this.put(entry.getKey(), entry.getValue());
		}
	}
}