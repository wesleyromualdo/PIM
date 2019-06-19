<?php

namespace Modules\Seguranca\Entities;

use Modules\Seguranca\Entities\Base\UsuarioSistemaBase;
use Yajra\Datatables\Datatables;

class UsuarioSistema extends UsuarioSistemaBase {

    const COD_SISTEMA = 264; //dev
   
    public function scopeAtivo($query) {
        return $query->where('susstatus', 'A')->orderBy('sisdsc', 'asc');
    }
    
    public function getList($request) {

        $collection = Sistema::select('sistema.sisid', 'sistema.sisdsc', 'sistema.sisdiretorio', 'us.suscod')
                ->leftjoin(UsuarioSistema::getTableName().' AS us' , function ($join) use ($request) {
                    $join->on('us.sisid', '=', 'sistema.sisid')
                    ->where('us.usucpf', '=', $request->usucpf);
                    //->where('pu.suscod', '=', 'A');
                 })->where('sisstatus', 'A')->orderBy('sisdsc', 'asc')->get();

        $datatables = Datatables::of($collection);

        return $datatables->make(true);

    }
    
    public function pegaPerfisSistemabyUsuario($request) {

        $perfilSistema = Perfil::select(['perfil.pflcod', 'perfil.pfldsc', 'pu.usucpf'])
                         ->leftjoin(PerfilUsuario::getTableName().' AS pu' , function ($join) use ($request) {
                            $join->on('pu.pflcod', '=', 'perfil.pflcod')
                            ->where('pu.usucpf', '=', $request->usucpf);
                            //->where('pu.suscod', '=', 'A');
                         }
                        )-> where(['perfil.pflstatus' => 'A', ['perfil.sisid', '=', $request->sisid] ])->get();
//        return UsuarioSistema::all()->limit(4);
        return $perfilSistema;
    }
    
    
    /*Override do metodo save que possibilita realizar o Update em tabelas com chave primaria composta*/
    public function save(array $options = [])
    {
        /*Verifica se a chave primaria é composta, caso não seja é chamado o metodo save original*/
        if( ! is_array($this->getKeyName()))
        {
            return parent::save($options);
        }

        // Fire Event for others to hook
        if($this->fireModelEvent('saving') === false) return false;

        // Prepare query for inserting or updating
        $query = $this->newQueryWithoutScopes();

        // Perform Update
        if ($this->exists)
        {
            if (count($this->getDirty()) > 0)
            {
                // Fire Event for others to hook
                if ($this->fireModelEvent('updating') === false)
                {
                    return false;
                }

                // Touch the timestamps
                if ($this->timestamps)
                {
                    $this->updateTimestamps();
                }

                //
                // START FIX
                //


                // Convert primary key into an array if it's a single value
                $primary = (count($this->getKeyName()) > 1) ? $this->getKeyName() : [$this->getKeyName()];

                // Fetch the primary key(s) values before any changes
                $unique = array_intersect_key($this->original, array_flip($primary));

                // Fetch the primary key(s) values after any changes
                $unique = !empty($unique) ? $unique : array_intersect_key($this->getAttributes(), array_flip($primary));

                // Fetch the element of the array if the array contains only a single element
                //$unique = (count($unique) <> 1) ? $unique : reset($unique);

                // Apply SQL logic
                $query->where($unique);

                //
                // END FIX
                //

                // Update the records
                $query->update($this->getDirty());

                // Fire an event for hooking into
                $this->fireModelEvent('updated', false);
            }
        }
        // Insert
        else
        {
            // Fire an event for hooking into
            if ($this->fireModelEvent('creating') === false) return false;

            // Touch the timestamps
            if($this->timestamps)
            {
                $this->updateTimestamps();
            }

            // Retrieve the attributes
            $attributes = $this->attributes;

            if ($this->incrementing && !is_array($this->getKeyName()))
            {
                $this->insertAndSetId($query, $attributes);
            }
            else
            {
                $query->insert($attributes);
            }

            // Set exists to true in case someone tries to update it during an event
            $this->exists = true;

            // Fire an event for hooking into
            $this->fireModelEvent('created', false);
        }

        // Fires an event
        $this->fireModelEvent('saved', false);

        // Sync
        $this->original = $this->attributes;

        // Touches all relations
        if (array_get($options, 'touch', true)) $this->touchOwners();

        return true;
    }
}