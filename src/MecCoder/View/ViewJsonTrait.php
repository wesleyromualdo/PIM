<?php
namespace MecCoder\View;

trait ViewJsonTrait
{

    /**
     * Método de publicação de um JSON
     * @param mixed $values
     */
    public function showJson($values)
    {
        //header('Content-Type: application/json');
        echo simec_json_encode($values);
        die;
    }
}
