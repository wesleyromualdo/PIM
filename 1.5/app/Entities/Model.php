<?php

namespace App\Entities;

use Illuminate\Database\Eloquent\Model as BaseModel;

class Model extends BaseModel {

    public $timestamps = false;
    public $rules = [];
    public $labels = [];


    public function scopeAtivo($query) {
        return $query->where(self::getTableName() . '.st_registro_ativo', 1);
    }

    public function getRules($rule, $json = false) {
        $rules = $this->rules;

        if (key_exists($rule, $rules)) {
            if ($json) {
                $var = [];

                foreach (explode('|', $this->rules[$rule]) as $key => $value) {
                    $f = explode(':', $value);
                    //TODO -- Rever tratamento das Rules para inculir todas as regras implementadas na model.
                    //if ($f[0] == 'required' || $f[0] =='unique') {
                    if (!key_exists(1, $f)) {
                        $var[$f[0]] = 1;
                    } else {
                        $var[$f[0]] = $f[1];
                    }
                }

                return json_encode($var);
            } else {
                return $this->rules[$rule];
            }
        } else {
            return null;
        }
    }

    public function getLabels($label) {
        $labels = $this->labels;

        if (key_exists($label, $labels)) {
            return $labels[$label];
        } else {
            return $label;
        }
    }

    public static function getTableName() {

        return with(new static)->getTable();
    }

    public function dumpSql($builder) {
        $sql = $builder->toSql();
        foreach ( $builder->getBindings() as $binding ) {
            $value = is_numeric($binding) ? $binding : "'".$binding."'";
            $sql = preg_replace('/\?/', $value, $sql, 1);
        }
        dd($sql);
    }


    function __destruct()
    {
        
        
//        if (!empty($_SESSION['usucpf']) && $_SESSION['usucpf'] === $_SESSION['usucpforigem']) {         
//            \DB::commit();
//        } else {
//            \DB::rollBack();
//        }
    }




}
