<?php

require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";



/**
 * 
 */
class entidades_fisicaBase.php extends ActiveRecord {
    /**
     * Nome da tabela especificada para esta entidade
     * @var string
     * @access protected
     */
    protected $tabela        = 'corporativo.entidades_fisica';

    /**
     * Sequence pertencente a tabela entidade.funcao
     * @access protected
     */
    protected $sequence      = 'corporativo.entidades_fisica_ensid_seq';

    /**
     * Chave primaria.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */
    protected $chavePrimaria = array('ensid', null);

    protected $campos        = array('ensdsc'      => null,
                                     'enfid' => null,
                                     'enjid'  => null,
                                     'ensstatus'      => null,
                                     'enscpf'   => null,
                                     'ensemail'      => null,
                                     'enstelefone'      => null,
                                     'ensendcep'    => null,
                                     'ensendlogradouro'       => null,
                                     'ensendcomplemento'        => null, 
                                     'ensendnumero'     => null,
                                     'ensendbairro' => null);



    /**
     * Carrega os dados recuperados do banco de dados no objeto.
     *
     * @param integer $id Valor da chave primária do registro no banco de dados
     */

    public function carregar($id = null)
    {
        if ($id == null)
            return new Arquivo();

        $sql = "SELECT
                    
                        ensid,
                        ensdsc,
                        enfid,
                        enjid,
                        ensstatus,
                        enscpf,
                        ensemail,
                        enstelefone,
                        ensendcep,
                        ensendlogradouro,
                        ensendcomplemento,
                        ensendnumero,
                        ensendbairro

                FROM
                    corporativo.entidades_fisica
                WHERE
                    ensid = ?";

        $rs = $this->Execute($sql, array($id));

                       $this->ensid             = $rs->fields['ensid'];
                       $this->ensdsc            = $rs->fields['ensdsc'];
                       $this->enfid             = $rs->fields['enfid'];
                       $this->enjid             = $rs->fields['enjid'];
                       $this->ensstatus         = $rs->fields['ensstatus'];
                       $this->enscpf            = $rs->fields['enscpf'];
                       $this->ensemail          = $rs->fields['ensemail'];
                       $this->enstelefone       = $rs->fields['enstelefone'];
                       $this->ensendcep         = $rs->fields['ensendcep'];
                       $this->ensendlogradouro  = $rs->fields['ensendlogradouro'];
                       $this->ensendcomplemento = $rs->fields['ensendcomplemento'];
                       $this->ensendnumero      = $rs->fields['ensendnumero'];
                       $this->ensendbairro      = $rs->fields['ensendbairro'];


        $this->chavePrimaria[1] = $rs->fields['ensid'];

        return clone $this;
    }


    /**
     * 
     */
    public function setPrimaryKey($valor)
    {
        return $this->chavePrimaria[1] = $valor;
    }


    /**
     * 
     */
    public function getPrimaryKey()
    {
        return $this->chavePrimaria[1];
    }
}





